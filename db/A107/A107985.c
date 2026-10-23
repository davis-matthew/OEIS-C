/*
 * A107985  Triangle read by rows: T(n,k) = (k+1)*(n+2)*(n-k+1)/2 for 0 <= k
 *          <= n.
 * https://oeis.org/A107985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: T(n,k) = (k+1)*(n+2)*(n-k+1)/2
 * Sequence a(n) is the row‑major reading of this triangle.
 * a(n) = T(r,c) where r = floor((sqrt(1+8n)-1)/2)
 * and c = n - r*(r+1)/2.
 */

#include <stdint.h>

/* compute the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t row_of_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* increase high until the triangular number exceeds n */
    while (((unsigned __int128)high * (high + 1)) / 2 <= n)
        high <<= 1;
    /* binary search for the largest r with triangular number <= n */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t c = n - start;

    seq_t val =
        (seq_t)(c + 1) *
        (seq_t)(r + 2) *
        (seq_t)(r - c + 1) / 2;

    return val;
}
