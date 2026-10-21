/*
 * A067447  Smallest n-th power starting with 6.
 * https://oeis.org/A067447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the smallest b^n (b ≥ 2) whose decimal representation
 * starts with the digit 6.  The index n is the exponent.
 *
 * The algorithm searches for the minimal base b such that the first digit
 * of b^n equals 6, using the logarithmic property
 *
 *  log10(b^n) = n * log10(b)
 *  first digit = ⌊10^( fractional part of n·log10(b) )⌋
 *
 *  The interval for which the leading digit is 6 is
 *      10^f ∈ [6,7)   ⇔   f ∈ [log10(6), log10(7)).
 *
 *  Once the appropriate base is found, the power is computed with the
 *  helper function seq_pow (provided by the build environment).
 */

#include <stdint.h>
#include <math.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    const long double L = log10l(6.0L);
    const long double U = log10l(7.0L);

    for (uint64_t b = 2;; ++b) {
        /* Compute fractional part of n * log10(b) */
        long double f = fmodl((long double)n * log10l((long double)b), 1.0L);

        /* Leading digit is 6 iff f lies in [L, U)  */
        if (f >= L && f < U) {
            /* Convert base to seq_t (fits as n ≤ 10000 and b is small) */
            seq_t base = (seq_t)b;
            /* Compute b^n as seq_t */
            return seq_pow(base, (int)n);
        }
    }
}
