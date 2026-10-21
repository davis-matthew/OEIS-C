/*
 * A239287  Triangle T(n,k), 0 <= k <= n, read by rows: T(n,k) = floor(n/2) -
 *          min(k,n-k).
 * https://oeis.org/A239287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(r/2) - min(k, r-k), * where r =
 *            floor((sqrt(8n+1)-1)/2) and k = n - r*(r+1)/2. * This expands
 *            the triangle T(n,k) = floor(n/2) – min(k,n-k) row‑wise
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    idx_t k = n - r * (r + 1) / 2;
    idx_t half = r >> 1;
    idx_t min_k = (k <= (r - k)) ? k : (r - k);
    return (seq_t)(half - min_k);
}
