/*
 * A180578  The Wiener index of the Dutch windmill graph D(6,n) (n>=1).
 * https://oeis.org/A180578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*(5*n-2)
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;          /* convert index to seq_t */
    seq_t term = 5 * N - 2;      /* 5*n - 2  */
    seq_t res = 9 * N;           /* 9*n     */
    res = res * term;            /* 9*n*(5*n-2) */
    return res;
}
