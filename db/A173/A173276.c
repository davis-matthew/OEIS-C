/*
 * A173276  a(n) = a(n-2) + a(n-3) - floor(a(n-3)/2) - floor(a(n-4)/2).
 * https://oeis.org/A173276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+5)/5) (OEIS A173276)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Perform the arithmetic in idx_t to avoid overflow on small seq_t types. */
    return (seq_t)(((idx_t)2 * n + 5) / 5);
}
