/*
 * A086009  Number of 1's in decimal expansion of n^2.
 * https://oeis.org/A086009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of '1' digits in decimal expansion of n^2
 * Range:     u8 n <= 19, u16 n <= 266, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t sq = (seq_t)n * (seq_t)n;      /* compute n² */
    seq_t count = 0;
    while (sq > 0) {                     /* count decimal digits that equal 1 */
        if (sq % 10 == 1)
            ++count;
        sq /= 10;
    }
    return count;
}
