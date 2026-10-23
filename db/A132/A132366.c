/*
 * A132366  Partial sum of centered tetrahedral numbers A005894.
 * https://oeis.org/A132366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^4 + 4*n^3 + 11*n^2 + 14*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 260, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                /* n as seq_t                     */
    seq_t n1 = sn + (seq_t)1;           /* n + 1                          */
    seq_t n1sq = n1 * n1;               /* (n + 1)^2                      */

    seq_t n2 = sn * sn;                 /* n^2                            */
    seq_t two_n = sn * (seq_t)2;        /* 2 * n                          */
    seq_t inner = n2 + two_n + (seq_t)6;/* n^2 + 2*n + 6                  */

    seq_t product = n1sq * inner;       /* (n + 1)^2 * (n^2 + 2*n + 6)   */
    seq_t result = product / (seq_t)6;  /* divide by 6                    */

    return result;
}
