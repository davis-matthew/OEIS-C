/*
 * A078268  Smallest integer which is an integer multiple of the number N
 *          obtained by placing the string "n" after a decimal point.
 * https://oeis.org/A078268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = n / gcd(n,10^{digits(n)})
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: remove, up to the number of decimal digits, all
   2‑factors and 5‑factors from n.  The remaining part is the answer. */

#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* n is at least 1.  Convert to seq_t. */
    seq_t m = (seq_t)n;

    /* Number of decimal digits of n. */
    uint64_t k = digit_count(n);

    /* Remove powers of 2, up to k times. */
    for (uint64_t i = 0; i < k && (m % 2 == 0); ++i) {
        m /= 2;
    }

    /* Remove powers of 5, up to k times. */
    for (uint64_t i = 0; i < k && (m % 5 == 0); ++i) {
        m /= 5;
    }

    return m;
}
