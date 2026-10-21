/*
 * A050407  a(n) = n*(n^2 - 6*n + 11)/6.
 * https://oeis.org/A050407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n^2 - 6*n + 11)/6
 * Range:     u8 n <= 13, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* cast index to sequence type */
    seq_t n2  = n_t * n_t;         /* n^2 */
    seq_t term = n2 - 6 * n_t + 11;/* n^2 - 6n + 11 */
    return n_t * term / 6;         /* final division (exact) */
}
