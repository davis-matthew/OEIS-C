/*
 * A128473  Numbers of the form 30*k+23 or numbers that cannot be part of a
 *          twin prime pair.
 * https://oeis.org/A128473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23 + 30*(n-1)
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n * (seq_t)30;   /* 30*n  */
    res -= (seq_t)30;                   /* subtract 30 -> 30*(n-1) */
    res += (seq_t)23;                   /* add 23  */
    return res;
}
