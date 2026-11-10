/*
 * A087317  Duplicate of A086489.
 * https://oeis.org/A087317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = smallest k such that k and k+n have the same prime
 *            signature (duplicate of A086489); computed by search
 * Range:     i8 n <= 33, i16 n <= 33, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static int A086489_cmp_u8(const void *p1, const void *p2)
{
    return (*(const unsigned char *)p1 - *(const unsigned char *)p2);
}

static void A086489_factor_exponents(uint64_t m, unsigned char exps[], int *len)
{
    *len = 0;
    unsigned char e;

    if (m % 2 == 0) {
        e = 0;
        while ((m % 2) == 0) {
            m /= 2;
            ++e;
        }
        exps[(*len)++] = e;
    }

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

    if (m > 1) {
        exps[(*len)++] = 1;
    }

    if (*len > 1) {
        qsort(exps, *len, sizeof(unsigned char), A086489_cmp_u8);
    }
}

static bool A086489_same_signature(uint64_t x, uint64_t y)
{
    unsigned char exp_a[24];
    unsigned char exp_b[24];
    int la, lb;

    A086489_factor_exponents(x, exp_a, &la);
    A086489_factor_exponents(y, exp_b, &lb);

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
        if (A086489_same_signature(k, k + nn)) {
            return (seq_t)k;
        }
    }
}
