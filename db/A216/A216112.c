/*
 * A216112  The Wiener index of the para-polyphenyl chain with n hexagons (see
 *          the Dou et al. and the Deng references).
 * https://oeis.org/A216112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(1+8*n^2)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. If n == 0 we
       return 0 (the value is undefined for n < 1 but the
       implementation may be called with n == 0). */
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;            /* Convert index to seq_t */
    seq_t n2 = N * N;              /* n^2 */
    seq_t term = 1 + 8 * n2;       /* 1 + 8*n^2 */
    return 3 * N * term;           /* 3*n*(1+8*n^2) */
}
