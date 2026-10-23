/*
 * A128471  a(n) = 30*n + 7.
 * https://oeis.org/A128471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 30*n + 7
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to seq_t to avoid overflow in intermediate calculations */
    return ((seq_t)n * (seq_t)30 + (seq_t)7);
}
