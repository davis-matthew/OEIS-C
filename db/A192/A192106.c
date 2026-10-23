/*
 * A192106  Decimal expansion of square root of 102.
 * https://oeis.org/A192106
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n‑th decimal digit of sqrt(102) with offset 2 (a(2) =
 *            first digit of the integer part 10). The sequence is the decimal
 *            expansion digits of sqrt(102) concatenated with the digits of
 *            the integer part. The following function implements a
 *            digit‑by‑digit square‑root algorithm using a small arbi...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Simple big integer type (little‑endian limbs, 32‑bit each). */
typedef struct {
    uint32_t *d;   /* limbs */
    size_t len;    /* used limbs */
    size_t cap;    /* allocated capacity */
} big;

/* Big integer operations -------------------------------------------------- */
static void big_init(big *b)
{
    b->d = NULL; b->len = 0; b->cap = 0;
}

static void big_free(big *b)
{
    free(b->d); b->d = NULL; b->len = 0; b->cap = 0;
}

static void big_ensure_cap(big *b, size_t needed)
{
    if (b->cap < needed) {
        size_t new_cap = b->cap ? b->cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint32_t *nd = (uint32_t *)realloc(b->d, new_cap * sizeof(uint32_t));
        if (!nd) return;           /* silently abort further growth on failure */
        b->d = nd; b->cap = new_cap;
    }
}

static void big_assign(const big *src, big *dst)
{
    big_ensure_cap(dst, src->len);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void big_mul_small(big *b, uint32_t m)
{
    if (m == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i]   = (uint32_t)prod;
        carry     = prod >> 32;
    }
    if (carry) {
        big_ensure_cap(b, b->len + 1);
        b->d[b->len] = (uint32_t)carry;
        b->len++;
    }
}

static void big_add_small(big *b, uint32_t a)
{
    if (a == 0) return;
    uint64_t sum = a;
    size_t i = 0;
    while (sum) {
        if (i < b->len) {
            sum += (uint64_t)b->d[i];
            b->d[i] = (uint32_t)sum;
        } else {
            big_ensure_cap(b, i + 1);
            b->d[i] = (uint32_t)sum;
            if (i + 1 > b->len) b->len = i + 1;
        }
        sum >>= 32;
        ++i;
    }
}

static int big_compare(const big *a, const big *b)
{
    if (a->len != b->len) return (int)(a->len > b->len) - (int)(a->len < b->len);
    size_t i = a->len;
    while (i) {
        --i;
        if (a->d[i] != b->d[i])
            return (int)(a->d[i] > b->d[i]) - (int)(a->d[i] < b->d[i]);
    }
    return 0;
}

static void big_sub_big(big *a, const big *b)   /* assume a >= b */
{
    uint64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (int64_t)b->d[i] - (int64_t)borrow;
        if (diff < 0) { diff += ((int64_t)1 << 32); borrow = 1; }
        else           borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    for (; borrow && i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (int64_t)borrow;
        if (diff < 0) { diff += ((int64_t)1 << 32); borrow = 1; }
        else           borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* ---------- Sequence state ------------------------------------------- */
static unsigned char *digits = NULL;   /* stored digits */
static size_t digits_len = 0;
static size_t digits_cap = 0;

static big R, D, C;                  /* working big integers */
static int  init_done = 0;

static const int groups[] = { 1, 2 };   /* groups of two digits of integer part of sqrt(102) */
static const int groups_cnt = 2;
static int group_index = 0;

static void ensure_digits(idx_t n)
{
    if (!init_done) {
        big_init(&R); big_init(&D); big_init(&C);
        init_done = 1;
    }
    size_t need = (size_t)(n - 1);     /* number of digits required */
    while (digits_len < need) {
        /* fetch next two‑digit group */
        int g;
        if (group_index < groups_cnt) {
            g = groups[group_index++];
        } else {
            g = 0;
        }

        /* R = R * 100 + g */
        big_mul_small(&R, 100u);
        big_add_small(&R, (uint32_t)g);

        /* find largest d: (D + d)*d <= R */
        int d;
        for (d = 9; d >= 0; --d) {
            big_assign(&D, &C);              /* C = D */
            big_add_small(&C, (uint32_t)d);  /* C = D + d */
            big_mul_small(&C, (uint32_t)d);  /* C = (D + d)*d */
            if (big_compare(&C, &R) <= 0) break;
        }

        /* update state */
        big_sub_big(&R, &C);              /* R -= C */
        big_mul_small(&D, 10u);           /* D *= 10 */
        big_add_small(&D, (uint32_t)(20u * (uint32_t)d)); /* D += 20*d */

        /* store digit */
        if (digits_len == digits_cap) {
            size_t new_cap = digits_cap ? digits_cap << 1 : 16;
            unsigned char *tmp = (unsigned char *)realloc(digits, new_cap);
            if (!tmp) return;             /* allocation failed: abort */
            digits = tmp;
            digits_cap = new_cap;
        }
        digits[digits_len++] = (unsigned char)d;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* below offset */
    ensure_digits(n);
    return (seq_t)digits[n - 2];
}
