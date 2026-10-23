/*
 * A195860  Minimal positive integer k such that n^k is not divisible by the
 *          sum of its decimal digits; or 0 if no such integer exists.
 * https://oeis.org/A195860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal positive integer k such that n^k is NOT divisible
 *            by the sum of its decimal digits; a(n)=0 for n=1 and for n=10^m
 *            (m≥0)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static void mul_small(uint8_t **digs, size_t *len, size_t *cap, uint64_t mul)
{
    __uint128_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        __uint128_t prod = (__uint128_t)(*digs)[i] * mul + carry;
        (*digs)[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        if (*len == *cap) {
            *cap *= 2;
            *digs = (uint8_t *)realloc(*digs, *cap);
        }
        (*digs)[*len] = (uint8_t)(carry % 10);
        carry /= 10;
        ++(*len);
    }
}

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 1) return (seq_t)0;

    /* n is a power of 10? */
    uint64_t t = n;
    while (t % 10 == 0) t /= 10;
    if (t == 1) return (seq_t)0;

    /* Convert base n to decimal digit array (least‑significant digit first) */
    size_t cap = 64;                 /* initial capacity */
    uint8_t *digs = (uint8_t *)malloc(cap);
    if (!digs) return (seq_t)0;      /* allocation failure unlikely */
    size_t len = 0;
    uint64_t v = n;
    if (v == 0) { digs[len++] = 0; } /* n≠0 in this problem, but keep for safety */
    else while (v > 0) {
        digs[len++] = (uint8_t)(v % 10);
        v /= 10;
    }

    size_t k = 1;
    for (;;) {
        /* Digit sum of n^k */
        uint64_t sum = 0;
        for (size_t i = 0; i < len; ++i) sum += digs[i];
        if (sum == 0) {          /* should not happen for n>0 */
            free(digs);
            return (seq_t)k;
        }

        /* n^k mod sum using the decimal digits */
        uint64_t rem = 0;
        for (size_t i = len; i > 0; --i) {
            rem = (rem * 10 + digs[i-1]) % sum;
        }

        if (rem != 0) {
            free(digs);
            return (seq_t)k;     /* condition satisfied */
        }

        /* Prepare for next power: multiply by n */
        mul_small(&digs, &len, &cap, n);
        ++k;
    }
}
