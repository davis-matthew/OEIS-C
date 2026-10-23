/*
 * A178977  a(n) = (3*n+2)*(3*n+5)/2.
 * https://oeis.org/A178977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+2)*(3*n+5)/2
 * Range:     u8 n <= 6, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;          /* convert index to seq_t */
    seq_t term1 = ns * 3 + 2;     /* 3*n + 2 */
    seq_t term2 = ns * 3 + 5;     /* 3*n + 5 */
    return term1 * term2 / 2;     /* division is exact */
}
