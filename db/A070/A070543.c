/*
 * A070543  Triangular array read by rows: T(n,k) = number of k-dimensional
 *          isotropic subspaces of Spin(2n+1,C), n >= 1, 1 <= k <= n.
 * https://oeis.org/A070543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = T(r,k) where * r = ceil( (sqrt(8n+1)-1)/2 ) (the row
 *            index, 1‑based) * k = n − r(r−1)/2 (the column index, 1‑based) *
 *            T(r,k) = k*(k+1)/2 + 2*k*(r−k)
 * Range:     u8 n <= 200, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Find the row r such that r(r+1)/2 ≥ n and (r−1)r/2 < n. */
static inline idx_t row_for_idx(idx_t n)
{
    idx_t low  = 1, high = 1;
    /* Increase high until it surely covers n */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    idx_t r     = row_for_idx(n);
    idx_t prev  = (idx_t)((__int128)(r - 1) * r / 2);
    idx_t k     = n - prev;

    seq_t K = (seq_t)k;
    seq_t R = (seq_t)r;

    return K * (K + 1) / 2 + (seq_t)2 * K * (R - K);
}
