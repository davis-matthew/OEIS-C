/*
 * A059540  Beatty sequence for 3^(1/3)/(3^(1/3)-1).
 * https://oeis.org/A059540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * 3^(1/3) / (3^(1/3) - 1) ) (Beatty sequence for
 *            3^(1/3)/(3^(1/3)-1) )
 * Range:     u8 n <= 78, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Compute the constant only once */
    static long double alpha = 0.0L;
    if (alpha == 0.0L) {
        long double c = powl(3.0L, 1.0L / 3.0L);           /* cube root of 3 */
        alpha = c / (c - 1.0L);                            /* α = c/(c-1)  */
    }

    /* Compute the floor of n*alpha using long double and convert to integer */
    uint64_t val = (uint64_t)floor((long double)n * alpha);

    return (seq_t)val;
}
