/*
 * A133082  Triangle read by rows: T(k,m) = dimension of shape space for k
 *          labeled points in R^m (1 <= k <= m-1, m >= 2).
 * https://oeis.org/A133082
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k*m - k*(k+1)/2 - 1, where (k,m) is obtained by reading
 *            the triangle T(k,m) = dimension of shape space for k labeled
 *            points in R^m in row‑major order, first row for m = 2. The k‑th
 *            column is an arithmetic progression with difference k
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle indices below the offset; behaviour is irrelevant. */
    if (n < 2) return (seq_t)0;

    /* ---------- find the row m that contains index n ----------
       The cumulative number of terms up to row m (inclusive) is
       C(m) = (m-1)*m/2.  We need the smallest m with C(m) >= n-1.
    */
    idx_t t = n - 1;                      /* number of terms before the first term of the triangle */
    idx_t s = 0;                          /* floor sqrt of 1 + 8*t */

    /* integer square root: floor(sqrt(x)) for 64‑bit unsigned x */
    {
        idx_t x = 1 + 8 * t;
        idx_t r = 0;
        for (int bit = 63; bit >= 0; --bit) {
            idx_t cand = r | (1ULL << bit);
            if ((unsigned __int128)cand * cand <= x) r = cand;
        }
        s = r;
    }

    idx_t m = (1 + s) >> 1;               /* provisional row */
    /* adjust if C(m) < t */
    if ((unsigned __int128)m * (m - 1) / 2 < t) ++m;

    /* first index of row m: 2 + (m-1)*(m-2)/2 */
    idx_t first = 2 + (unsigned __int128)(m - 1) * (m - 2) / 2;

    /* position within the row (0‑based) */
    idx_t pos = n - first;
    idx_t k   = pos + 1;                  /* 1 <= k <= m-1 */

    /* ----- compute T(k,m) = k*m - k*(k+1)/2 - 1 ---- */
    seq_t km = (seq_t)k;
    seq_t mm = (seq_t)m;
    seq_t part1 = km * mm;
    seq_t part2 = km * (km + 1) / 2;
    seq_t result = part1 - part2 - (seq_t)1;

    return result;
}
