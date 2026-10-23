/*
 * A019559  Period 5: repeat [4, 4, 6, 6, 6].
 * https://oeis.org/A019559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*floor((n+5)/5)+2*floor((n+3)/5)-6*floor(n/5)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic implementation for a(n) = [4,4,6,6,6] repeating every 5 terms. */
seq_t a(idx_t n)
{
    idx_t m = n % 5;        /* n mod 5 */
    return (m < 2) ? (seq_t)4 : (seq_t)6;
}
