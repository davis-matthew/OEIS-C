/*
 * A056837  Duplicate of A001971.
 * https://oeis.org/A056837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ((n-1)/2) * ((n+1)/2) / 2 (n odd) ; a(n) = ((n/2) * (n/2)
 *            / 2) + ((n/2) mod 2) (n even)
 * Range:     i8 n <= 23, i16 n <= 25, i32 n <= 25, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1) == 0) {                 /* n is even */
        seq_t m = (seq_t)(n >> 1);      /* m = n/2 */
        seq_t sq = m * m;               /* m*m, indiv. seq_t multiplication */
        seq_t half = sq / 2;            /* floor(m*m/2) */
        return half + (seq_t)(m & 1);   /* add m mod 2 */
    } else {                            /* n is odd */
        seq_t m = (seq_t)((n - 1) >> 1); /* m = (n-1)/2 */
        return m * (m + 1) / 2;          /* m*(m+1)/2 */
    }
}
