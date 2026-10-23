/*
 * A008816  Expansion of (1+x^9)/((1-x)^2*(1-x^9)).
 * https://oeis.org/A008816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) + 2*floor(n/9)*(n+1) - 9*floor(n/9)*(floor(n/9)+1)
 * Range:     u8 n <= 46, u16 n <= 766, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t term = (seq_t)(n + 1);          /* n+1  */
    seq_t m    = (seq_t)(n / 9);          /* floor(n/9) */
    return term + 2 * m * term - 9 * m * (m + 1);
}
