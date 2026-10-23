/*
 * A131579  Period 10: repeat 0, 3, 6, 9, 2, 5, 8, 1, 4, 7.
 * https://oeis.org/A131579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n) mod 10
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 10 to avoid overflow. */
    idx_t r = n % 10;
    /* Compute 3*r mod 10 using only allowed integer operations. */
    seq_t val = (seq_t)((r * 3) % 10);
    return val;
}
