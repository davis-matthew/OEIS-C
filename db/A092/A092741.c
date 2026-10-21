/*
 * A092741  Triangle read by rows: T(n,k) is the number of permutations p of
 *          [n] in which the length of the longest initial segment avoiding
 *          both the 132- and the 321-pattern is equal to k.
 * https://oeis.org/A092741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row such that r(r+1)/2 ≥ n >
 *            (r−1)r/2, k = n − (r−1)r/2. T(r,k) = 0 for k=1 (except r=1),
 *            T(r,r) = r(r−1)/2 + 1, and for 2 ≤ k < r T(r,k) = (∏_{i=k−1}^{r}
 *            i) · k / (2·(k+1))
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   triangle_entry
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangle_entry.h"

/* Find the row r (≥1) such that r(r+1)/2 ≥ n > (r−1)r/2. */
static inline idx_t find_row(idx_t n)
{
    /* Binary search on a monotone function.  Use __int128 for safety. */
    idx_t low  = 1;
    idx_t high = 1;
    while (true) {
        unsigned __int128 val = (unsigned __int128)high * (high + 1) / 2;
        if (val >= n) break;
        high <<= 1;                      /* exponential growth */
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 midv = (unsigned __int128)mid * (mid + 1) / 2;
        if (midv >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

/* Public function computing the sequence term a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below the offset is undefined */

    idx_t r = find_row(n);           /* the row containing the n‑th entry */
    unsigned __int128 prev_sum = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_sum;   /* column within that row */

    return triangle_entry(r, k);
}
