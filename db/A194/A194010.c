/*
 * A194010  Mirror of the triangle A194009.
 * https://oeis.org/A194010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the rows of the triangle w(r,k) in
 *            reverse order, where w(r,k) = s_k * r + d_k, with s_0 = 1, s_1 =
 *            2, s_k = s_{k-1} + s_{k-2} + 1 (k ≥ 2); d_0 = 2, d_1 = 3, and
 *            for k ≥ 2, d_k = s_{k-2} + (k + 2)
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r that contains index n in the flattened sequence.
   Uses binary search on the triangular numbers   r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t row_of_idx(idx_t n)
{
    idx_t low = 0;
    /* an upper bound larger than all possible row numbers for 64‑bit indices */
    idx_t high = (idx_t)1 << 32;          /* > sqrt(2·2^64) */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* locate the row r and position within that row */
    idx_t r      = row_of_idx(n);
    idx_t first  = r * (r + 1) / 2;     /* start index of row r */
    idx_t offset = n - first;           /* 0-based offset in the row */
    idx_t k      = r - offset;          /* column index in the original triangle */

    /* compute slope s_k and intercept d_k */
    seq_t slope, d;

    if (k == 0) {
        slope = 1;
        d     = 2;
    } else if (k == 1) {
        slope = 2;
        d     = 3;
    } else {
        /* recurrence for s_k : s_k = s_{k-1} + s_{k-2} + 1 */
        seq_t s_prevprev = 1;   /* s_0 */
        seq_t s_prev     = 2;   /* s_1 */
        seq_t s_cur;
        seq_t s_k_minus2 = 1;   /* will hold s_{k-2} when i reaches k-1 */

        for (idx_t i = 2; i <= k; ++i) {
            s_cur = s_prev + s_prevprev + 1;
            if (i == k - 1)
                s_k_minus2 = s_prev;   /* s_{k-2} */
            if (i == k)
                slope = s_cur;        /* s_k */
            s_prevprev = s_prev;
            s_prev     = s_cur;
        }

        /* d_k = s_{k-2} + (k + 2)  for k >= 2 */
        d = s_k_minus2 + (k + 2);
    }

    /* w(r,k) = s_k * r + d_k */
    return slope * (seq_t)r + d;
}
