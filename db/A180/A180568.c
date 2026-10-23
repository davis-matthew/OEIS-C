/*
 * A180568  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the grid P_3 x P_n (1<=k<=n), where P_j
 *          denotes the path graph on j nodes.
 * https://oeis.org/A180568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+3)/2 ≥ n;
 *            k = n - (r-1)(r+2)/2. T(r,k) counts unordered pairs of vertices
 *            at distance k in the grid P_3 × P_r, expressed by T(r,k) =
 *            3·max(r-k,0) + 2·f(k-1) + f(k-2), with f(d) = { r, if d = 0
 *            2·(r-d), if d > 0, 0, otherwise } for 0 ≤ d ≤ r-1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#  include <cstdint>
#else
#  include <stdint.h>
#endif

static inline seq_t compute_T(idx_t n, idx_t k)
{
    if (k < 1 || k > n + 1)  return (seq_t)0;

    seq_t res   = 0;
    seq_t ss_n  = (seq_t)n;
    seq_t ss_k  = (seq_t)k;

    /* same-row contributions (Δr = 0, d = k) */
    if (k < n) {
        res += (seq_t)3 * (ss_n - ss_k);        /* 3·(n-k)  */
    }

    /* pairs between rows with Δr = 1 (d = k-1) */
    if (k <= n) {
        if (k == 1) {
            res += (seq_t)2 * ss_n;            /* 2·n   when d = 0 */
        } else {
            seq_t diff = ss_n - (ss_k - 1);     /* n-(k-1) */
            res += (seq_t)4 * diff;            /* 4·(n-k+1)  */
        }
    }

    /* pairs between rows with Δr = 2 (d = k-2) */
    if (k >= 2 && k <= n + 1) {
        if (k == 2) {
            res += ss_n;                       /* n   when d = 0 */
        } else {
            seq_t diff = ss_n - (ss_k - 2);     /* n-(k-2) */
            res += (seq_t)2 * diff;            /* 2·(n-k+2)  */
        }
    }

    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+3)/2 ≥ n  (using binary search). */
    idx_t low  = 1;
    idx_t high = n;          /* n is the index; high = index is ample. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 S = (unsigned __int128)mid * (mid + 3) / 2;
        if (S >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    idx_t r = low;

    /* Compute the position k within row r. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * (r + 2) / 2;
    idx_t k = n - (idx_t)prev;          /* 1 ≤ k ≤ r+1 */

    return compute_T(r, k);
}
