/*
 * A139264  a(n) = 70*n - 63.
 * https://oeis.org/A139264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 70*n - 63
 * Range:     u8 n <= 4, u16 n <= 937, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* Undefined for n = 0, but harmless */
    return ((seq_t)n * 70) - 63;   /* Compute in seq_t to avoid overflow */
}
