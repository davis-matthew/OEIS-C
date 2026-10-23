/*
 * A168107  a(n) = sum of natural numbers m such that n - 8 <= m <= n + 8.
 * https://oeis.org/A168107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+8)*(n+9)/2 for 0 <= n <= 8; a(n) = (n+8)*(n+9)/2 -
 *            (n-9)*(n-8)/2 for n > 8
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n + 8);
    seq_t top = m * (m + 1) / 2;           /* (n+8)*(n+9)/2 */
    if (n > (idx_t)8) {
        seq_t l = (seq_t)(n - 9);
        seq_t bottom = l * (l + 1) / 2;   /* (n-9)*(n-8)/2 */
        return top - bottom;
    }
    return top;
}
