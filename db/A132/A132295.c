/*
 * A132295  Sum of the nonsquare numbers not larger than n.
 * https://oeis.org/A132295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - r*(r+1)*(2*r+1)/6, where r = floor(sqrt(n))
 * Range:     u8 n <= 23, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_idx(idx_t n) {
    /* Integer square root using binary search.  Works for any n ≥ 0. */
    idx_t lo = 0, hi = (n >> 1) + 1, mid, r = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid * mid <= n) {
            r = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    /* Sum of 1..n: n*(n+1)/2 */
    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;

    /* r = floor(sqrt(n)) */
    idx_t r_idx = isqrt_idx(n);
    seq_t r = (seq_t)r_idx;

    /* Sum of squares of 1..r: r*(r+1)*(2*r+1)/6 */
    seq_t square_sum = r * (r + 1) * (2 * r + 1) / 6;

    /* Desired sum of nonsquare numbers */
    return total - square_sum;
}
