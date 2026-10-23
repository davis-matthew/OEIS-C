/*
 * A131475  a(n) = floor(n/2) * floor(n^2/2).
 * https://oeis.org/A131475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) * floor(n^2/2)
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* cast index to value type */
    seq_t half_n = n_seq >> 1;       /* floor(n/2) */
    seq_t n_sq   = n_seq * n_seq;    /* n^2 (fits if final result fits) */
    seq_t half_n_sq = n_sq / 2;      /* floor(n^2/2) */
    return half_n * half_n_sq;       /* product */
}
