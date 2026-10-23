/*
 * A151978  Numbers that are congruent to {0, 1} mod 17.
 * https://oeis.org/A151978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17 * ((n-1) / 2) + ((n-1) & 1)
 * Range:     u8 n <= 31, u16 n <= 7711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 0, a(2) = 1, a(3) = 17, ... using the offset 1 */
    idx_t k = n - 1;          /* k >= 0 for n >= 1 */
    idx_t m = k / 2;          /* floor(k/2) */
    seq_t result = (seq_t)17 * (seq_t)m + (seq_t)(k & 1);
    return result;
}
