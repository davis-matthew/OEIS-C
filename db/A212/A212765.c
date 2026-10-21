/*
 * A212765  Number of (w,x,y,z) with all terms in {0,...,n}, w even and x, y,
 *          and z odd.
 * https://oeis.org/A212765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n^4 + 6*n^3 + 6*n^2 + 4*n + 1) - (2*n^3 + 6*n^2 + 4*n
 *            + 1)*(-1)^n) / 32 Simplified: if n is even : a(n) = (n^4 +
 *            2*n^3) / 16 if n is odd : a(n) = (n + 1)^4 / 16
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    if ((n & 1) == 0) {            /* n even */
        seq_t n_sq = sn * sn;      /* n²   */
        seq_t n_cu = n_sq * sn;    /* n³   */
        seq_t n_qu = n_cu * sn;    /* n⁴   */
        return (n_qu + n_cu * 2) / 16; /* (n⁴ + 2 n³)/16 */
    } else {                       /* n odd  */
        seq_t np1 = sn + 1;         /* n+1   */
        seq_t np1_sq = np1 * np1;   /* (n+1)² */
        seq_t np1_cu = np1_sq * np1;/* (n+1)³ */
        seq_t np1_qu = np1_cu * np1;/* (n+1)⁴ */
        return np1_qu / 16;          /* (n+1)⁴/16 */
    }
}
