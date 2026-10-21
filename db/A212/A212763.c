/*
 * A212763  Number of (w,x,y,z) with all terms in {0,...,n}, and w, x and y
 *          odd.
 * https://oeis.org/A212763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*n^3/8 for even n; * a(n) = 2*((n+1)/2)^4 for odd n
 *            (equivalently (n+1)^4/8)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t N = n;
    if ((N & 1) == 0) {                    /* n even */
        idx_t np1 = N + 1;                 /* n+1 */
        idx_t n2   = N * N;                /* n^2   */
        idx_t n3_8 = (n2 * N) / 8;         /* n^3/8, integer because n even */
        return (seq_t)(np1 * n3_8);        /* (n+1)*n^3/8 */
    } else {                               /* n odd */
        idx_t np1 = N + 1;                 /* n+1 (even) */
        idx_t m    = np1 >> 1;             /* (n+1)/2 */
        idx_t m2   = m * m;                /* m^2   */
        idx_t m4   = m2 * m2;              /* m^4   */
        return (seq_t)(2 * m4);            /* 2*m^4 = (n+1)^4/8 */
    }
}
