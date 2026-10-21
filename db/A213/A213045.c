/*
 * A213045  Number of (w,x,y) with all terms in {0,...,n} and 2*|w-x| >
 *          max(w,x,y) - min(w,x,y).
 * https://oeis.org/A213045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(4*n^2 + 9*n + 8) - 3*(-1)^n + 3) / 12
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to the result type */
    seq_t N = (seq_t)n;

    /* Compute the cubic polynomial part */
    seq_t Nsq  = N * N;                     /* n^2                     */
    seq_t part = 4 * Nsq + 9 * N + 8;        /* 4*n^2 + 9*n + 8        */

    /* Full numerator: 2*n*part + adjustment for (-1)^n */
    seq_t num = 2 * N * part;               /* 2*n*(4*n^2 + 9*n + 8)  */
    seq_t adjust = (n & 1) ? 6 : 0;          /* 6 when n is odd, else 0 */
    seq_t numerator = num + adjust;         /* final numerator         */

    /* Divide by 12 – integral division is exact */
    return numerator / 12;
}
