/*
 * A212565  Number of (w,x,y,z) with all terms in {1,...,n} and w+x>=2y+2z.
 * https://oeis.org/A212565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n^4 - 12*n^3 - 8*n^2 - 6*((-1)^n-1)*n + 3*((-1)^n-1))
 *            / 96 * (Equivalently: a(n) = (14*n^4 - 12*n^3 - 8*n^2 + (n odd ?
 *            12*n-6 : 0)) / 96)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 191, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;               /* convert index to seq_t */
    seq_t m2 = m * m;                 /* n^2 */
    seq_t m3 = m2 * m;                /* n^3 */
    seq_t m4 = m2 * m2;               /* n^4 */

    /* compute polynomial 14*n^4 - 12*n^3 - 8*n^2 */
    seq_t poly = 14 * m4;
    poly -= 12 * m3;
    poly -= 8 * m2;

    /* add odd–term adjustments: 12*n - 6 when n is odd, else nothing */
    if (n & 1U) {
        poly += 12 * m;
        poly -= 6;
    }

    return poly / (seq_t)96;
}
