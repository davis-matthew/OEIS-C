/*
 * A157458  Triangle, read by rows, double tent function: T(n,k) = min(1 +
 *          2*k, 1 + 2*(n-k), n).
 * https://oeis.org/A157458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = min(1+2*k, 1+2*(r-k), r) where r =
 *            floor((sqrt(8*n+1)-1)/2) is the row index and k = n - r*(r+1)/2
 *            is the position within the row. This implements OEIS A157458,
 *            the triangle read by rows using a double tent function
 * Range:     u8 n <= 9999, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t triangular_idx(idx_t n)
{
    /* Binary search for the greatest r such that r*(r+1)/2 <= n. */
    unsigned __int128 lo = 0, hi = 1;
    /* Expand hi until it exceeds the needed range. */
    while (hi * (hi + 1) / 2 <= (unsigned __int128)n) {
        hi <<= 1;
    }
    /* Standard binary search on the interval [lo, hi). */
    while (lo < hi) {
        unsigned __int128 mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= (unsigned __int128)n) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    /* lo is now the smallest value with lo*(lo+1)/2 > n, so r = lo - 1 */
    return (idx_t)(lo - 1);
}

seq_t a(idx_t n)
{
    /* Determine the row index r and the within‑row position k. */
    idx_t r = triangular_idx(n);
    seq_t t_r = (seq_t)r * ((seq_t)r + 1) / 2;   /* triangular number of r */
    idx_t k = n - (idx_t)t_r;                    /* offset in the row */

    /* Evaluate the three expressions and take the minimum. */
    seq_t v1 = (seq_t)1 + (seq_t)2 * (seq_t)k;
    seq_t v2 = (seq_t)1 + (seq_t)2 * ((seq_t)r - (seq_t)k);
    seq_t v3 = (seq_t)r;

    seq_t res = v1;
    if (v2 < res) res = v2;
    if (v3 < res) res = v3;
    return res;
}
