/*
 * A174337  a(n) = 94*n^2.
 * https://oeis.org/A174337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 94*n^2
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 6759, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast the index to seq_t before arithmetic to avoid overflow on idx_t */
    seq_t sn = (seq_t)n;
    return 94u * sn * sn;
}
