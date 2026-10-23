/*
 * A143977  Rectangular array R by antidiagonals: label each unit square in
 *          the first quadrant lattice by its northeast vertex (x,y) and mark
 *          squares having |x-y| == 0 (mod 3); then R(m,n) is the number of
 *          marked squares in the rectangle [0,m] X [0,n].
 * https://oeis.org/A143977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ceil(m*n/3), where (m,n) are obtained from n by mapping
 *            the antidiagonal order of the array R(m,n) with m,n>=1
 * Range:     u8 n <= 955, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1; undefined below */
    idx_t s = 2;                         /* diagonal index (m+n) */
    idx_t cum = 1;                       /* entries up to diagonal s */
    for (;;) {                           /* find smallest s with cum >= n */
        if (cum >= n) break;
        ++s;
        cum = s * (s - 1) / 2;
    }
    idx_t prev_cum = (s - 1) * (s - 2) / 2;
    idx_t pos = n - prev_cum;            /* 1‑based position on this diagonal */
    idx_t m = s - pos;                   /* m decreases along the diagonal */
    idx_t n_coord = pos;                 /* second coordinate */
    seq_t prod = (seq_t)m * (seq_t)n_coord;
    return (prod + 2) / 3;               /* ceil division by 3 */
}
