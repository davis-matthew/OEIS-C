/*
 * A101383  a(n) = n*(n+1)*(2*n^3 - n^2 + 2)/6.
 * https://oeis.org/A101383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n^3 - n^2 + 2)/6
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 105, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * (nn + 1) / 2;          /* n*(n+1)/2   */
    seq_t nsq = nn * nn;                   /* n^2          */
    seq_t ncu = nsq * nn;                  /* n^3          */
    seq_t expr = (seq_t)2 * ncu - nsq + (seq_t)2; /* 2*n^3 - n^2 + 2 */

    /* Divide the product by 3 while keeping all intermediate results in seq_t */
    if (t1 % 3 == 0)
        t1 /= 3;
    else
        expr /= 3;

    return t1 * expr;                      /* final result */
}
