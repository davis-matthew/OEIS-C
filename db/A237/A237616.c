/*
 * A237616  a(n) = n*(n + 1)*(5*n - 4)/2.
 * https://oeis.org/A237616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n - 4)/2
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    seq_t tri   = n_t * (n_t + 1) / 2;            /* n*(n+1)/2   */
    seq_t factor = (seq_t)5 * n_t - (seq_t)4;      /* 5*n - 4     */
    return tri * factor;                          /* a(n)         */
}
