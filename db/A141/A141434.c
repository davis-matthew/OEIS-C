/*
 * A141434  Triangle T(n, k) = (k-1)*(3*n-k-1), read by rows.
 * https://oeis.org/A141434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the n‑th term of the triangle T(r,k) = (k-1)*(3*r-k-1)
 *            read by rows
 * Range:     u8 n <= 88, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find r such that r*(r-1)/2 < n <= r*(r+1)/2 */
    idx_t lo = 1;
    idx_t hi = (idx_t)1 << 33;            /* upper bound for r (sqrt(2*max_n)) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* Column within the row: c = n - (r-1)*r/2 */
    __int128 cum = (__int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)cum;

    /* Compute T(r,c) = (c-1)*(3*r-c-1) */
    seq_t r_t = (seq_t)r;
    seq_t c_t = (seq_t)c;
    return (c_t - (seq_t)1) * (((seq_t)3 * r_t) - c_t - (seq_t)1);
}
