/*
 * A202107  a(n) = n^4*(n+1)^4/8.
 * https://oeis.org/A202107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n+1)^4/8 (equivalent to 2*(n*(n+1)/2)^4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* sequence is defined from n >= 1 */

    /* compute the triangular number t = n*(n+1)/2  */
    seq_t n_t = (seq_t)n;
    seq_t t   = n_t * (n_t + 1) / 2;

    /* compute t^4 = (t^2)^2  */
    seq_t t2 = t * t;
    seq_t t4 = t2 * t2;

    /* final result  a(n) = 2 * t^4  */
    return t4 * 2;
}
