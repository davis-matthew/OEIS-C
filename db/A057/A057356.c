/*
 * A057356  a(n) = floor(2*n/7).
 * https://oeis.org/A057356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/7)
 * Range:     u8 n <= 895, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(2*n/7) using only integer arithmetic
       and cast the index to seq_t before performing multiplications
       to avoid temporary overflow on small sequence types. */
    seq_t q = (seq_t)(n / 7);           /* n div 7 */
    seq_t r = (seq_t)(n % 7);           /* n mod 7 */
    return q * 2 + (r * 2) / 7;         /* floor(2*n/7) */
}
