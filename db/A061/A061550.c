/*
 * A061550  a(n) = (2*n+1)*(2*n+3)*(2*n+5).
 * https://oeis.org/A061550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)*(2*n+3)*(2*n+5)
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 811, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* cast the index to seq_t */
    seq_t m  = nn * 2;        /* m = 2*n, computed in seq_t to avoid overflow */
    return (m + 1) * (m + 3) * (m + 5);
}
