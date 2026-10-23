/*
 * A144917  a(n) is the maximal odd value attained by A144916(n).
 * https://oeis.org/A144917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n - 11 for n>=3, with a(1)=1, a(2)=3
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 3;
    return ((seq_t)n * 6) - 11;
}
