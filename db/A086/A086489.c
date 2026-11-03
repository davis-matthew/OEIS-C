/*
 * A086489  Smallest k such that k and k + n have the same prime signature.
 * https://oeis.org/A086489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k and k+n have the same prime
 *            signature, i.e. the multiset of exponents in the prime
 *            factorisations of k and k+n are identical
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 113 terms through n = 9597 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int cmp_u8(const void *p1, const void *p2)
{
    return (*(const unsigned char *)p1 - *(const unsigned char *)p2);
}

static void factor_exponents(uint64_t m, unsigned char exps[], int *len)
{
    *len = 0;
    unsigned char e;

    /* factor out 2 */
    if (m % 2 == 0) {
        e = 0;
        while ((m % 2) == 0) {
            m /= 2;
            ++e;
        }
        exps[(*len)++] = e;
    }

    /* factor out odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[(*len)++] = e;
        }
    }

    /* remaining prime factor > sqrt(original m) */
    if (m > 1) {
        exps[(*len)++] = 1;
    }

    /* sort exponents ascending for signature comparison */
    if (*len > 1) {
        qsort(exps, *len, sizeof(unsigned char), cmp_u8);
    }
}

static bool same_signature(uint64_t a, uint64_t b)
{
    static unsigned char exp_a[20];
    static unsigned char exp_b[20];
    int la, lb;

    factor_exponents(a, exp_a, &la);
    factor_exponents(b, exp_b, &lb);

    if (la != lb) return false;

    for (int i = 0; i < la; ++i) {
        if (exp_a[i] != exp_b[i]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    for (uint64_t k = 1; ; ++k) {
        if (same_signature(k, k + nn)) {
            return (seq_t)k;
        }
    }
}
