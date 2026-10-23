/*
 * A040977  a(n) = binomial(n+5,5)*(n+3)/3.
 * https://oeis.org/A040977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5)*(n+3)/3
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 104, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_uint64.h"

/* Compute gcd of two unsigned 64-bit integers */

seq_t a(idx_t n)
{
    /* factors: (n+1),(n+2),(n+3),(n+3),(n+4),(n+5) */
    const unsigned int offs[6] = {1, 2, 3, 3, 4, 5};

    seq_t prod = 1;          /* numerator so far   */
    uint64_t den = 360;      /* denominator to divide by */

    for (unsigned i = 0; i < 6; ++i) {
        seq_t fac = (seq_t)(n + offs[i]);

        /* Simplify fac with the remaining denominator */
        uint64_t rem_fac = (uint64_t)(fac % den);
        uint64_t g = gcd_uint64(rem_fac, den);
        if (g > 1) {
            fac = fac / g;
            den /= g;
        }

        /* Simplify current product with the remaining denominator */
        if (den > 1) {
            uint64_t rem_prod = (uint64_t)(prod % den);
            uint64_t g2 = gcd_uint64(den, rem_prod);
            if (g2 > 1) {
                prod = prod / g2;
                den /= g2;
            }
        }

        /* Multiply by the reduced factor */
        prod = prod * fac;
    }

    /* Any remaining denominator must divide prod exactly */
    if (den > 1)
        prod = prod / den;

    return prod;
}
