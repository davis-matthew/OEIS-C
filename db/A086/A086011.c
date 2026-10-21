/*
 * A086011  Number of 3's in decimal expansion of n^2.
 * https://oeis.org/A086011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 323, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of digit '3' in the decimal representation of n^2
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute n^2 using seq_t arithmetic */
    seq_t sq = (seq_t)n * ((seq_t)n);

    /* Count digits equal to 3 */
    seq_t count = 0;
    while (sq > (seq_t)0) {
        if ((sq % (seq_t)10) == (seq_t)3)
            count = count + (seq_t)1;
        sq = sq / (seq_t)10;
    }
    return count;
}
