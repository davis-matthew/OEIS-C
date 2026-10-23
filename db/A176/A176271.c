/*
 * A176271  The odd numbers as a triangle read by rows.
 * https://oeis.org/A176271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*n-1
 * Range:     u8 n <= 128, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first, then apply the formula. */
    return ((seq_t)2 * (seq_t)n) - (seq_t)1;
}
