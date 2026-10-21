/*
 * A067865  Numbers n such that n and 2^n end with the same two digits.
 * https://oeis.org/A067865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 36 + 100*(n-1) (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 36 + 100*(n-1) = 100*n - 64 */
    seq_t result = (seq_t)100 * (seq_t)n - (seq_t)64;
    return result;
}
