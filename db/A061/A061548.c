/*
 * A061548  Numerator of probability that there is no error when average of n
 *          numbers is computed, assuming errors of +1, -1 are possible and
 *          they each occur with p = 1/4.
 * https://oeis.org/A061548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(binomial(4*n,2*n)/2^(4*n)) = binomial(4*n,2*n)
 *            / 2^v2(binomial(4*n,2*n)) = product of odd parts of numbers
 *            1..4n divided by the square of the product of odd parts of
 *            numbers 1..2n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* Prepare arrays up to 4*n */
    idx_t limit = n << 2;          /* 4*n */
    size_t sz = (size_t)limit + 1; /* include 0 */

    /* Smallest prime factor array */
    uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (spf == NULL) return 0;
    for (size_t i = 0; i < sz; ++i) spf[i] = 0;

    for (size_t i = 2; i < sz; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i < sz) {
                for (size_t j = i * i; j < sz; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Exponent differences of odd prime factors:
       exp[p] = exponent of p in odd part of (4n)! – 2 * exponent in odd part of (2n)!   */
    int *exp = (int *)calloc(sz, sizeof(int));
    if (exp == NULL) { free(spf); return 0; }

    /* Process odd parts of numbers 1..4n */
    for (idx_t i = 2; i <= (idx_t)limit; ++i) {
        idx_t x = i;
        while ((x & 1ULL) == 0) x >>= 1;   /* remove factors of 2 */
        while (x > 1) {
            uint32_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            exp[p] += cnt;
        }
    }

    /* Subtract twice the odd part exponents of numbers 1..2n */
    idx_t half = n << 1;           /* 2*n */
    for (idx_t i = 2; i <= half; ++i) {
        idx_t x = i;
        while ((x & 1ULL) == 0) x >>= 1;
        while (x > 1) {
            uint32_t p = spf[x];
            int cnt = 0;
            while (x % p == 0) { x /= p; ++cnt; }
            exp[p] -= 2 * cnt;
        }
    }

    seq_t result = 1;
    for (uint32_t p = 2; p < sz; ++p) {
        int e = exp[p];
        if (e > 0) {
            result *= pow_int((seq_t)p, e);
        }
    }

    free(exp);
    free(spf);
    return result;
}
