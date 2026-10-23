/*
 * A138015  Triangle read by rows, antidiagonals of an array formed by A000012
 *          * A136579. Replace the term "n" in the correlation triangle
 *          A003983 with A003422(n).
 * https://oeis.org/A138015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A003422( min(d, r-1-d)+1 ) where r is the unique integer
 *            with r(r+1)/2 ≥ n > (r-1)r/2 and d = n - (r-1)r/2 - 1 is the
 *            zero‑based offset in row r. A003422(m) = Σ_{i=0}^{m-1} i!
 *            (partial sums of factorials)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the row r such that T(r-1) < n ≤ T(r), T(r)=r*(r+1)/2 */
    idx_t lo = 1, hi = 1;
    /* Grow hi until T(hi) ≥ n (safe with __int128 to avoid overflow) */
    while (( (__int128)hi * (hi + 1) ) / 2 < n) {
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                            /* r ≥ 1 */
    __int128 tri_prev = (__int128)(r - 1) * r / 2; /* T(r-1) */
    idx_t d = (idx_t)(n - (idx_t)tri_prev - 1);    /* zero‑based offset in row */

    /* distance from the nearest end of the row */
    idx_t k = (d < (r - 1 - d)) ? d : (r - 1 - d);
    idx_t m = k + 1;                         /* index used for A003422 */

    /* Compute A003422(m) = Σ_{i=0}^{m-1} i! */
    seq_t sum = 0;
    seq_t fact = 1;
    for (idx_t i = 0; i < m; ++i) {
        sum += fact;
        fact *= (i + 1); /* multiply by next integer to get (i+1)! */
    }
    return sum;
}
