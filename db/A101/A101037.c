/*
 * A101037  Triangle read by rows: T(n,1) = T(n,n) = n and for 1<k<n: T(n,k) =
 *          floor((T(n-1,k-1)+T(n-1,k))/2).
 * https://oeis.org/A101037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * The sequence is triangle read by rows with * * T(n,1) = T(n,n)
 *            = n * T(n,k) = floor((T(n-1,k-1)+T(n-1,k))/2) for 1<k<n * * A
 *            closed form is: * * T(n,k) = n - 2*k + 2 if 2*k < n+1 * T(n,k) =
 *            2 if n is odd and 2*k == n+1 * T(n,k) = 2*k - n otherwise * *
 *            The sequence a(n) is the triangle read by r...
 * Range:     u8 n <= 9187, u16 n <= 9187, u32 n <= 9187, u64 n <= 9187,
 *            u128 n <= 9187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row number r such that (r-1)*r/2 < n <= r*(r+1)/2 */
    idx_t lo = 1;
    /* r <= sqrt(2*n) < 2^33 for n up to 2^64-1 */
    idx_t hi = 1ULL << 33;          /* 8589934592 */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t >= n) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    idx_t r = lo;

    unsigned __int128 tPrev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t tPrev = (idx_t)tPrev128;            /* tPrev < n <= 2^64-1 */
    idx_t k = n - tPrev;                      /* 1 <= k <= r */

    if (r == 1) {
        return (seq_t)1;
    }

    /* Closed‑form formula for T(r,k) */
    if ((r & 1) && 2 * k == r + 1) {
        return (seq_t)2;
    }
    if ((unsigned __int128)2 * k < (unsigned __int128)r + 1) {
        return (seq_t)r - 2 * (seq_t)k + 2;
    } else {
        return 2 * (seq_t)k - (seq_t)r;
    }
}
