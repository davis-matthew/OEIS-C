/*
 * A093577  Decimal expansion of (3/4)*sqrt(2).
 * https://oeis.org/A093577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A093577: decimal expansion of (3/4)*sqrt(2)
   a(n) = floor( ( (3/4)*sqrt(2) ) * 10^(n-1) ) % 10
   The algorithm computes the digits of sqrt(18) = 3*sqrt(2) and
   divides each accumulated value by 4.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BIG_BASE 1000000000U          /* 10^9 */
#define BIG_DIGITS 9

typedef struct {
    size_t len;        /* number of used words, at least 1 */
    uint32_t *d;       /* little‑endian: d[0] least significant word */
} Big;

/* Allocate a big integer with enough capacity.  */
static void big_init(Big *b, size_t maxwords)
{
    b->len = 1;
    b->d   = (uint32_t*)calloc(maxwords, sizeof(uint32_t));
    if (!b->d) abort();
}

/* Free a big integer.  */
static void big_free(Big *b)
{
    free(b->d);
}

/* Set big integer to unsigned value.  */
static void big_set_uint(Big *b, uint32_t val)
{
    size_t i = 0;
    if (val == 0) {
        b->len = 1;
        b->d[0] = 0;
        return;
    }
    while (val) {
        b->d[i++] = val % BIG_BASE;
        val /= BIG_BASE;
    }
    b->len = i;
}

/* Copy src into dest.  */
static void big_copy(Big *dest, const Big *src)
{
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}

/* Compare two big ints.  */
static int big_cmp(const Big *a, const Big *b)
{
    if (a->len != b->len)
        return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

/* Subtract b from a (a>=b), store in out.  */
static void big_sub(const Big *a, const Big *b, Big *out)
{
    out->len = a->len;
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = a->d[i];
        int64_t bi = (i < b->len) ? b->d[i] : 0;
        int64_t sub = ai - bi - borrow;
        if (sub < 0) { sub += BIG_BASE; borrow = 1; }
        else          borrow = 0;
        out->d[i] = (uint32_t)sub;
    }
    while (out->len > 1 && out->d[out->len - 1] == 0)
        --out->len;
}

/* Multiply big by small m, store in out.  */
static void big_mul_small(const Big *a, uint32_t m, Big *out)
{
    if (m == 0) {
        big_set_uint(out, 0);
        return;
    }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        out->d[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    if (carry) {
        out->d[i++] = (uint32_t)carry;
    }
    out->len = i;
}

/* Multiply in place by small m.  */
static void big_mul_small_inplace(Big *a, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIG_BASE);
        carry = prod / BIG_BASE;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Add small d to big in place.  */
static void big_add_small_inplace(Big *a, uint32_t d)
{
    uint64_t carry = d;
    size_t i = 0;
    for (; i < a->len; ++i) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
        if (!carry) break;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Divide big by small d (d>0) in place, return remainder.  */
static uint32_t big_div_small_inplace(Big *a, uint32_t d, uint32_t *rem_out)
{
    uint64_t rem = 0;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        uint64_t cur = a->d[i] + rem * BIG_BASE;
        a->d[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0)
        --a->len;
    *rem_out = (uint32_t)rem;
    return (uint32_t)rem;
}

/* Return the nth digit of (3/4)*sqrt(2).  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    /* Allocate big integers with enough capacity */
    size_t words = (n + 12) / BIG_DIGITS + 3;   /* safety margin */
    Big P, R, temp, cand, prod;
    big_init(&P, words);
    big_init(&R, words);
    big_init(&temp, words);
    big_init(&cand, words);
    big_init(&prod, words);

    /* Initial values for sqrt(18) = 3*sqrt(2) */
    big_set_uint(&P, 4);    /* floor(sqrt(18)) = 4 */
    big_set_uint(&R, 2);    /* 18 - 4^2 = 2 */

    for (idx_t i = 0; i < n; ++i) {
        /* Compute digit: floor(P / 4) % 10 */
        big_copy(&temp, &P);
        uint32_t rem;
        big_div_small_inplace(&temp, 4, &rem);   /* temp now holds floor(P/4) */
        seq_t digit = (seq_t)(temp.d[0] % 10);
        if (i == n - 1) {
            /* Clean up */
            big_free(&P);
            big_free(&R);
            big_free(&temp);
            big_free(&cand);
            big_free(&prod);
            return digit;
        }

        /* Prepare next digit of sqrt(18) */
        big_mul_small_inplace(&R, 100);          /* R = R * 100 */

        int d;
        for (d = 9; d >= 0; --d) {
            big_copy(&cand, &P);
            big_mul_small_inplace(&cand, 20);    /* 20*P */
            big_add_small_inplace(&cand, d);     /* 20*P + d */
            big_mul_small(&cand, d, &prod);      /* (20*P + d)*d */
            if (big_cmp(&prod, &R) <= 0) break;
        }

        /* Update remainder and P */
        big_sub(&R, &prod, &R);
        big_mul_small_inplace(&P, 10);
        big_add_small_inplace(&P, d);
    }

    /* Should never reach here */
    big_free(&P);
    big_free(&R);
    big_free(&temp);
    big_free(&cand);
    big_free(&prod);
    return (seq_t)0;
}
