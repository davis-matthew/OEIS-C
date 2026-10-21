/*
 * A218832  Number of positive integer solutions to the Diophantine equation x
 *          + y + 2z = n^2.
 * https://oeis.org/A218832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^4/4 - n^2 + 1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Promote the index to the sequence type before any arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute n^2 and n^4 using the sequence type to avoid overflow
       for the supported sizes. */
    seq_t n2 = sn * sn;       /* n^2 */
    seq_t n4 = n2 * n2;       /* n^4 */

    /* Integer division handles the floor automatically.
       All constants are cast to seq_t for type consistency. */
    seq_t res = n4 / (seq_t)4;   /* floor(n^4 / 4) */
    res = res - n2 + (seq_t)1;   /* subtract n^2 and add 1 */

    return res;
}
