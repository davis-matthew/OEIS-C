/*
 * A086079  Number of 8's in decimal expansion of triangular number n(n+1)/2.
 * https://oeis.org/A086079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of digits '8' in the decimal expansion of
 *            n*(n+1)/2
 * Range:     u8 n <= 26, u16 n <= 3963, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the triangular number as a seq_t */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Count digits equal to 8 */
    seq_t count = 0;
    while (t > 0) {
        if (t % 10 == 8)
            ++count;
        t /= 10;
    }

    return count;
}
