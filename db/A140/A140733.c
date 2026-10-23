/*
 * A140733  Triangle read by rows, X^n * [1,0,0,0,...]; where X = a
 *          tridiagonal matrix with (1,1,1,...) in the main and subdiagonals
 *          and (1,2,3,...) in the subsubdiagonal.
 * https://oeis.org/A140733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry read left‑to‑right in the triangle
 *            defined by the recurrence T_r[i] = T_{r-1}[i] + T_{r-1}[i-1] (i
 *            ≥ 1) + (i-1)*T_{r-1}[i-2] (i ≥ 2) where T_1 = [1]. The element
 *            a(n) is the entry in row r = ceil(√n) at 0‑based position p = n
 *            – (r‑1)² – 1
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* floor(√x) for 0 ≤ x < 2^64 */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;            /* √(2^64‑1) < 2^32 */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* safety for indices below offset */

    /* Locate the row and position of a(n) */
    idx_t r   = isqrt_u64(n - 1) + 1;            /* ceil(√n)  */
    idx_t start = (r - 1) * (r - 1);             /* 0‑based start of row r */
    idx_t p      = n - start - 1;                /* 0‑based position in that row */

    if (r == 1) return (seq_t)1;                 /* first row */

    /* current row (prev) and its length */
    seq_t *prev = alloc_seq_array(1);
    if (!prev) return (seq_t)0;
    prev[0] = (seq_t)1;
    size_t prev_len = 1;

    for (idx_t row = 2; row <= r; ++row) {
        size_t new_len = 2 * row - 1;
        seq_t *curr = alloc_seq_array(new_len);
        if (!curr) { free_seq_array(prev); return (seq_t)0; }

        for (size_t i = 0; i < new_len; ++i) {
            seq_t a_val = (i < prev_len) ? prev[i] : (seq_t)0;
            seq_t b_val = (i >= 1 && (i - 1) < prev_len) ? prev[i - 1] : (seq_t)0;
            seq_t c_val = (i >= 2 && (i - 2) < prev_len) ? prev[i - 2] : (seq_t)0;

            seq_t val = a_val;
            if (i >= 1) val += b_val;
            if (i >= 2) val += (seq_t)(i - 1) * c_val;
            curr[i] = val;
        }

        free_seq_array(prev);
        prev      = curr;
        prev_len  = new_len;
    }

    seq_t result = prev[p];
    free_seq_array(prev);
    return result;
}
