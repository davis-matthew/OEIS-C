/*
 * A068074  a(n) = Sum_{d|n} (-1)^d*2^omega(n/d) where omega(x) is the number
 *          of distinct prime factors in the factorization of x.
 * https://oeis.org/A068074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * 2^omega(n/d) (OEIS A068074)
 * Range:     i8 n <= 1679, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Work with an unsigned 64‑bit copy of n */
    uint64_t nn = (uint64_t)n;

    /* Count the exponent of 2.  For n=0 we return 0 as a harmless
       default, though the sequence is defined only for n>=1. */
    unsigned e2 = 0;
    while ((nn & 1ULL) == 0) {
        e2++;
        nn >>= 1;
    }

    /* nn is now the odd part of the input.  Compute the product
       of (2*exp+1) over all odd prime factors. */
    uint64_t odd = nn;
    seq_t prod = 1;

    for (uint64_t p = 3; p * p <= odd; p += 2) {
        if (odd % p == 0) {
            unsigned exp = 0;
            while (odd % p == 0) {
                odd /= p;
                exp++;
            }
            prod = prod * (seq_t)(2 * exp + 1);
        }
    }
    if (odd > 1) {          /* remaining prime factor with exp = 1 */
        prod = prod * (seq_t)3;   /* 2*1+1 = 3 */
    }

    /* Return the value, handling the odd/even case.  For odd n we
       return the negative of the divisor count of n²; for even n
       the coefficient (2*e2-3) multiplies the same product. */
    if (e2 == 0) {
        return -(seq_t)prod;
    } else {
        int64_t coeff = (int64_t)(2ULL * e2 - 3ULL);
        return prod * (seq_t)coeff;
    }
}
