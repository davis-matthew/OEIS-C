/*
 * A139601  Square array of polygonal numbers read by ascending antidiagonals:
 *          T(n, k) = (n + 1)*(k - 1)*k/2 + k.
 * https://oeis.org/A139601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,j) where T(i,j) = (i+1)*(j-1)*j/2 + j , and (i,j) are
 *            the coordinates of the element read by antidiagonals: i + j = d
 *            with d chosen so that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2
 * Range:     u8 n <= 128, u16 n <= 1274, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal index d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially increase high until it exceeds n. */
    while (true) {
        /* Use 128‑bit arithmetic for the comparison to avoid overflow. */
        if ((__int128)high * (high + 1) / 2 > n)
            break;
        high <<= 1;          /* double until we cross n */
    }
    /* Binary search to find the exact floor d. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;                           /* anti‑diagonal index */
    idx_t tri_start = d * (d + 1) / 2;       /* first index on this diagonal */
    idx_t k = n - tri_start;                /* position along the diagonal   */
    idx_t i = d - k;                         /* row index   */
    idx_t j = k;                             /* column index */

    /* Small elements (j == 0) are zero in the array definition. */
    if (j == 0)
        return (seq_t)0;

    /* Compute part = j*(j-1)/2 and then the full value. */
    seq_t part = (seq_t)j * (seq_t)(j - 1) / 2;      /* j*(j-1)/2 */
    seq_t val  = (seq_t)(i + 1) * part + (seq_t)j;   /* (i+1)*part + j */

    return val;
}
