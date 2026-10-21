/*
 * A099538  Sum of the first n digits of sqrt(2), including the initial "1".
 * https://oeis.org/A099538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum of the first n decimal digits of sqrt(2) (including
 *            the leading 1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Working big integer base 1e9 */
#define BASE 1000000000U
#define MAX_WORDS 1200   /* enough for 10000 decimal digits (≈1112 words) */

typedef struct {
    unsigned len;                 /* number of used words, least‑significant first */
    uint32_t d[MAX_WORDS];
} Big;

///----- basic bigint operations ----///
static void bigint_zero(Big *a) { a->len = 0; }

static void bigint_set_uint(Big *a, uint32_t v) {
    if (v == 0) { a->len = 0; return; }
    a->d[0] = v; a->len = 1;
}

static void bigint_copy(Big *dst, const Big *src) {
    dst->len = src->len;
    for (unsigned i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

static int bigint_cmp(const Big *a, const Big *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i)
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    return 0;
}

static void bigint_mul_uint(Big *a, uint32_t m) {
    if (m == 1 || a->len == 0) return;
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (unsigned i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    while (carry) {
        if (a->len >= MAX_WORDS) return;   /* overflow guard, never happens for our limits */
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void bigint_add_uint(Big *a, uint32_t v) {
    if (v == 0) return;
    uint64_t carry = v;
    unsigned i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
        ++i;
    }
    while (carry) {
        if (a->len >= MAX_WORDS) return;  /* overflow guard, never happens for our limits */
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static void bigint_sub(Big *a, const Big *b) { /* assumes a >= b */
    uint64_t borrow = 0;
    for (unsigned i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow - (i < b->len ? b->d[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

static void bigint_sub_small(Big *a, uint32_t v) { /* a -= v, v < BASE */
    if (v == 0 || a->len == 0) return;
    uint64_t borrow = v;
    unsigned i = 0;
    while (borrow && i < a->len) {
        uint64_t cur = (uint64_t)a->d[i];
        if (cur >= borrow) {
            a->d[i] = (uint32_t)(cur - borrow);
            borrow = 0;
        } else {
            a->d[i] = (uint32_t)(cur + BASE - borrow);
            borrow = 1;
        }
        ++i;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

///----- sequence function ----///
seq_t a(idx_t n) {
    /* n ≥ 1 (offset 1) */
    if (n == 0) return (seq_t)0;

    Big root, R, temp, root20;
    bigint_zero(&root);
    bigint_zero(&R);
    seq_t sum = 0;

    /* First group (single digit 2) */
    bigint_set_uint(&R, 2ULL);
    /* Find first digit d such that d^2 ≤ R (R=2) */
    uint32_t d = 1;  /* obvious, since 1^2=1 ≤2, 2^2=4>2 */
    bigint_set_uint(&root, d);
    bigint_sub_small(&R, d * d);   /* R -= d*d  */
    sum += (seq_t)d;               /* first digit contributes to sum */

    /* Subsequent digits (n-1 groups of "00") */
    for (idx_t i = 1; i < n; ++i) {
        /* Bring down the next group '00' */
        bigint_mul_uint(&R, 100ULL);   /* multiply remainder by 100 */

        /* Compute 20 * root once */
        bigint_copy(&root20, &root);
        bigint_mul_uint(&root20, 20ULL);

        /* Find next digit d (0..9) */
        for (d = 9; d <= 9; --d) {   /* loop downward 9..0 */
            bigint_copy(&temp, &root20);
            bigint_add_uint(&temp, d);
            bigint_mul_uint(&temp, d);         /* temp = (root20 + d)*d */
            if (bigint_cmp(&temp, &R) <= 0) break;
        }

        /* Update remainder and root */
        bigint_sub(&R, &temp);                 /* R -= temp */
        bigint_mul_uint(&root, 10ULL);         /* root *= 10 */
        bigint_add_uint(&root, d);             /* root += d */

        /* Accumulate digit sum */
        sum += (seq_t)d;
    }

    return sum;
}
