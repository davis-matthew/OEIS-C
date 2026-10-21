/*
 * A222657  a(n) = 2 * floor( (2*n + 1) / 3) + 1.
 * https://oeis.org/A222657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * floor( (2*n + 1) / 3) + 1
 * Range:     u8 n <= 137, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n+1 using seq_t to avoid overflow */
    seq_t t = (seq_t)n * 2 + 1;
    /* Integer division gives floor((2*n+1)/3) */
    seq_t r = (t / 3) * 2 + 1;
    return r;
}
