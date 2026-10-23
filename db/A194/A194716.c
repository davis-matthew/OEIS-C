/*
 * A194716  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting four doublets into the
 *          initially empty word.
 * https://oeis.org/A194716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 6*(n-1) + 14*n*(n-1)^2 (a(0)=0)
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 675, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t N = (seq_t)n;
    seq_t k = (seq_t)(n - 1);            /* k = n-1, positive for n>0 */
    seq_t term = 1 + 6 * k;              /* add 1 + 6*(n-1) */
    /* compute 14 * n * k * k   (using intermediate variables to be safe) */
    seq_t tmp = N * k;                   /* N*k  */
    tmp = tmp * k;                      /* N*k*k */
    tmp = tmp * 14;                     /* 14*N*k*k */
    term += tmp;
    return term;
}
