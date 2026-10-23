/*
 * A026037  a(n) = dot_product(1,2,...,n)*(3,4,...,n,1,2).
 * https://oeis.org/A026037
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 3*n + 13)/6
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A026037: dot_product(1,2,...,n) * (3,4,...,n,1,2) for n >= 3 */

seq_t a(idx_t n)
{
    /* The offset of the sequence is 3; values for n < 3 are unspecified.
       Returning 0 for such n keeps the function safe but these cases are
       not tested. */
    if (n < 3)
        return (seq_t)0;

    seq_t nn = (seq_t)n;                 /* cast the index to seq_t */
    seq_t n2 = nn * nn;                  /* n^2 */
    seq_t term = (seq_t)2 * n2            /* 2*n^2   */
                - (seq_t)3 * nn          /*   -3*n  */
                + (seq_t)13;             /*   +13   */

    /* Final result: n * term / 6 */
    return nn * term / (seq_t)6;
}
