/*
 * A157870  a(n) = (4*n+1)*(4*n+2) = (4*n+2)!/(4*n)!.
 * https://oeis.org/A157870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(4*n+2)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 4*n+1 safely in seq_t, then multiply by (x+1) */
    seq_t x = (seq_t)n * 4 + 1;
    return x * (x + 1);
}
