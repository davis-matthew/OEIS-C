/*
 * A132000  Expansion of (1/3) * b(q) * b(q^2) * c(q)^2 / c(q^2) in powers of
 *          q where b(), c() are cubic AGM functions.
 * https://oeis.org/A132000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -b(n) with b multiplicative: b(1) = 1 b(2^e) = 2 +
 *            ((-4)^(e+1)-1)/5 b(3^e) = 1 for odd prime p>3, let s =
 *            Kronecker(-3,p) (±1) then b(p^e) = ( (p^2*s)^(e+1) - 1 ) /
 *            (p^2*s - 1)
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 1915, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9680 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static int kronecker_minus3(uint64_t p) /* p is an odd prime > 3 */
{
    /* Legendre symbol (−1/p) */
    int l1 = (p % 4 == 1) ? 1 : -1;
    /* Legendre symbol (3/p) using quadratic reciprocity */
    int l3 = (p % 12 == 1 || p % 12 == 11) ? 1 : -1;
    return l1 * l3; /* product gives (−3/p) */
}

static seq_t b_of_uint64(uint64_t n)
{
    if (n == 1) return (seq_t)1;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(n, pr, ex, &cnt);
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t part;
        if (p == 2) {
            seq_t term = (seq_t)(-4);
            seq_t pow = (seq_t)1;
            for (uint64_t j = 0; j < e + 1; ++j) pow *= term;
            part = (seq_t)2 + (pow - (seq_t)1) / (seq_t)5;
        } else if (p == 3) {
            part = (seq_t)1;
        } else {
            int kr = kronecker_minus3(p);
            seq_t q = (seq_t)p * (seq_t)p * (seq_t)kr;
            seq_t pow = (seq_t)1;
            for (uint64_t j = 0; j < e + 1; ++j) pow *= q;
            part = (pow - (seq_t)1) / (q - (seq_t)1);
        }
        prod *= part;
    }
    return prod;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    return -b_of_uint64((uint64_t)n);
}
