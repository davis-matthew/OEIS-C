/*
 * A047387  Numbers that are congruent to {1, 2, 5} mod 7.
 * https://oeis.org/A047387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*((n-1)/3) + {1,2,5}[(n-1)%3]
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A047387: Numbers congruent to 1, 2 or 5 mod 7 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial offset: a(1) should be the first term */
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Compute block (number of full groups of 3) and position inside the group */
    idx_t block   = (n - 1) / 3;
    idx_t pos     = (n - 1) % 3;

    /* Residues in sorted order for each block */
    static const uint8_t res[3] = {1, 2, 5};

    /* Compute the value using seq_t arithmetic */
    seq_t val = (seq_t)7U * (seq_t)block + (seq_t)res[pos];
    return val;
}
