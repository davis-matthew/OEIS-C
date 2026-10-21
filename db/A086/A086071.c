/*
 * A086071  Number of 0's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of 0 digits in the decimal expansion of n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* compute the triangular number */
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    
    /* special case: tri == 0 has one zero digit */
    if (tri == 0)
        return 1;
    
    seq_t zeros = 0;
    while (tri > 0) {
        if ((tri % 10) == 0)
            ++zeros;
        tri /= 10;
    }
    return zeros;
}
