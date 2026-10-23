/*
 * A120888  Triangle read by rows: T(n,k) = gcd(k,floor(n/k)) (1 <= k <= n).
 * https://oeis.org/A120888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value at the n-th entry of the triangle T(r,k) =
 *            gcd(k, floor(r/k)) (rows r = 1,2,…; columns k = 1,…,r), where n
 *            counts the entries row‑by‑row, starting with a(1) = T(1,1)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function maps n to (r,k) with r chosen such that
   r(r+1)/2 ≥ n > (r-1)r/2, then k = n - (r-1)r/2.  */
/* Requires:  ../utils/gcd.h */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Find smallest r with r(r+1)/2 >= n.  */
    idx_t lo = 1, hi = 1;
    unsigned __int128 sum;

    while ((sum = (unsigned __int128)hi * (hi + 1) / 2) < n)
        hi <<= 1;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;   /* row number */

    /* Compute previous triangle sum t_prev = r(r-1)/2.  */
    idx_t t_prev = (unsigned __int128)r * (r - 1) / 2;

    /* Column index within the row.  */
    idx_t k = n - t_prev;           /* 1 <= k <= r */

    /* floor(r/k) */
    idx_t m = r / k;

    return gcd((seq_t)k, (seq_t)m);
}
