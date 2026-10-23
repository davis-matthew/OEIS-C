/*
 * A024191  [ (3rd elementary symmetric function of 3,4,...,n+4)/(3+4+...+n+4)
 *          ].
 * https://oeis.org/A024191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n*n + 13*n + 46)/24 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 374, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t before any arithmetic */
    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;
    seq_t N2 = N * N + (seq_t)13 * N + 46;   /* n^2 + 13n + 46 */

    seq_t num = N * N1;   /* n*(n+1) */
    num = num * N2;       /* n*(n+1)*(n^2+13n+46) */

    return num / 24;      /* exact division by 24 */
}
