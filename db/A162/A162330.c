/*
 * A162330  Blocks of 4 numbers of the form 2k, 2k-1, 2k, 2k+1, k=1,2,3,4,...
 * https://oeis.org/A162330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 - 2*floor((n + 2)/4)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based index; the formula works for n >= 1 */
    idx_t t = (n + 2) / 4;          /* floor((n+2)/4) */
    seq_t result = (seq_t)n + 1 - (seq_t)2 * (seq_t)t;
    return result;
}
