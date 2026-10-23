/*
 * A156810  a(n) = 225*n^2 - 251*n + 70.
 * https://oeis.org/A156810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 225*n^2 - 251*n + 70
 * Range:     u8 n <= 1, u16 n <= 17, u32 n <= 4369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_ts = (seq_t)n;
    seq_t term_sq  = n_ts * n_ts;                 /* n^2 */
    seq_t term_225 = (seq_t)225 * term_sq;        /* 225*n^2 */
    seq_t term_251 = (seq_t)251 * n_ts;           /* 251*n */
    return term_225 - term_251 + (seq_t)70;       /* a(n) = 225n^2 - 251n + 70 */
}
