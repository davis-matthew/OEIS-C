/*
 * A056203  Triangle of numbers related to congruum problem:
 *          T(n,k)=n^2+2nk-k^2 with n>k>0, starting at T(2,1)=7.
 * https://oeis.org/A056203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k)=n^2+2*n*k-k^2, with r and k derived
 *            from the index n as described above
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n) {
    /* Find the smallest m such that m*(m+1)/2 >= n.
       Here m = r-1 where r is the row number. */
    idx_t low = 0;
    idx_t high = 1;
    while (((__int128)high * (high + 1)) / 2 < n)
        high <<= 1;

    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;          /* m = r-1 */
}

seq_t a(idx_t n) {
    /* Locate the row r and column k for the n-th term. */
    idx_t m = find_row(n);          /* m = r-1 */
    idx_t r = m + 1;                /* row number */

    /* Cumulative count up to row r-1: (m-1)*m/2 */
    idx_t prev_cum = (idx_t)(((unsigned __int128)(m - 1) * m) / 2);
    idx_t offset0 = n - prev_cum - 1;   /* zero‑based index within the row */
    idx_t k = offset0 + 1;              /* 1‑based column number */

    /* Compute T(r,k) = r^2 + 2*r*k - k^2 in seq_t arithmetic. */
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;

    seq_t val = r_seq * r_seq + (seq_t)2 * r_seq * k_seq - k_seq * k_seq;
    return val;
}
