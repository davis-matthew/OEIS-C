/*
 * A144679  a(n) = floor(n/5 + 1) * floor(n/5 + 2) * (3*n - 10*floor(n/5) +
 *          3)/6.
 * https://oeis.org/A144679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(n/5 + 1) * floor(n/5 + 2) * (3*n - 10*floor(n/5) +
 *            3) / 6
 * Range:     u8 n <= 12, u16 n <= 173, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute q = floor(n/5) */
    idx_t q = n / 5;

    /* (q+1)*(q+2) */
    seq_t first = (seq_t)(q + 1) * (seq_t)(q + 2);

    /* (3*n - 10*q + 3) */
    seq_t third = (seq_t)3 * (seq_t)n - (seq_t)10 * (seq_t)q + (seq_t)3;

    /* product */
    seq_t prod = first * third;

    /* divide by 6 */
    return prod / (seq_t)6;
}
