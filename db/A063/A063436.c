/*
 * A063436  Write 1, 2, 3, 4, ... counterclockwise in a hexagonal spiral
 *          around 0 starting left down, then a(n) is the sequence found by
 *          reading from 0 in the vertical upward direction.
 * https://oeis.org/A063436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(4*n+1)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 3 * nn * (4 * nn + 1);
}
