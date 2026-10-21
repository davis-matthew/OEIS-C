/*
 * A054389  Susceptibility series H_5 for 2-dimensional Ising model (divided
 *          by 2).
 * https://oeis.org/A054389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(154*n^4 + 1260*n^2 + 941 - 45*(-1)^n)/120, a(0) = 1.
 *            The division by 120 is exact for all n ≥ 0
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t nn = (seq_t)n;                  /* n as seq_t */

    /* Compute n^2 .. n^5 */
    seq_t n2 = nn * nn;                   /* n^2 */
    seq_t n3 = n2 * nn;                   /* n^3 */
    seq_t n4 = n3 * nn;                   /* n^4 */
    seq_t n5 = n4 * nn;                   /* n^5 */

    /* Base part: 154*n^5 + 1260*n^3 + 941*n */
    seq_t term = (seq_t)154 * n5 + (seq_t)1260 * n3 + (seq_t)941 * nn;

    /* Adjust by 45*n*(-1)^n:
       if n even: subtract 45*n
       if n odd : add 45*n  */
    if ((n & 1) == 0) {                   /* n is even */
        term = term - (seq_t)45 * nn;
    } else {                               /* n is odd  */
        term = term + (seq_t)45 * nn;
    }

    /* Exact division by 120 */
    return term / (seq_t)120;
}
