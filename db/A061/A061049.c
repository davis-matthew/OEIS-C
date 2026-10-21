/*
 * A061049  Numerator of 1/64 - 1/n^2.
 * https://oeis.org/A061049
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of (1/64 - 1/n²) after simplifying
 * Range:     u8 none, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute numerator and denominator of the fraction 1/64 - 1/n² */
    seq_t num = sn * sn - (seq_t)64;
    seq_t den = (seq_t)64 * sn * sn;

    /* Reduce the fraction and return the simplified numerator */
    seq_t g = gcd(num, den);
    return num / g;
}
