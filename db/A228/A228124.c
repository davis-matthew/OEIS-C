/*
 * A228124  Number of blocks in a Steiner Quadruple System of order
 *          A047235(n+1).
 * https://oeis.org/A228124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*(1+3*n)*(1+3*(-1)^n+6*n))/16
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 633, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; this case is not used but safe */
        return 0;

    seq_t n_t   = (seq_t)n;
    seq_t t2    = n_t * 3 + 1;               /* 1 + 3*n   */
    seq_t t3;                               /* 1 + 3*(-1)^n + 6*n */
    if ((n_t & 1) == 0)
        t3 = n_t * 6 + 4;                   /* even n : 4 + 6*n */
    else
        t3 = n_t * 6 - 2;                   /* odd  n : -2 + 6*n */

    seq_t num = n_t * t2;        /* n*(1+3*n)  */
    num = num * t3;              /* * (1+3*(-1)^n+6*n) */
    return num / 16;
}
