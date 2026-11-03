/*
 * A120914  Cascadence of (1+2x)^2; a triangle, read by rows of 2n+1 terms,
 *          that retains its original form upon convolving each row with
 *          [1,4,4] and then letting excess terms spill over from each row
 *          into the initial positions of the next row such that only 2n+1
 *          terms remain in row n for n>=0.
 * https://oeis.org/A120914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 36, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A120914 – Cascadence of (1+2x)^2
   The triangle is built by iterating a convolution with [1,4,4].
   Row 0 = [1].
   For k ≥ 0:
       let conv = row(k) * [1,4,4]   (ordinary linear convolution);
       let next[0] = next[2k+2] = conv[1];
       for 1 ≤ i ≤ 2k+1:  next[i] = conv[i+1].
   The flattened sequence a(n) is obtained by reading the rows in order.
   Initial terms:
       a(0)=1, a(1)=4, a(2)=4, a(3)=4, a(4)=20, a(5)=36, …
   This implementation proceeds row‑by‑row up to the row containing n,
   using only additions, multiplications by 4, and zeroing an auxiliary
   buffer. */

#include <stdlib.h>                 /* calloc, free */
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n) {
    /* Determine the row r and the position pos within that row */
    idx_t r   = isqrt_u64(n);                 /* row index, r² ≤ n < (r+1)² */
    idx_t pos = n - r * r;                    /* 0‑based index in row r */

    /* Base case: the very first element */
    if (r == 0) return (seq_t)1;

    /* Largest number of elements in any row up to r: 2r+1 */
    size_t max_len = (size_t)(2 * r + 1);

    /* Allocate three working buffers of that size */
    seq_t *curr = mem_new(seq_t, max_len);
    seq_t *next = mem_new(seq_t, max_len);
    seq_t *conv = mem_new(seq_t, max_len);
    if (!curr || !next || !conv) {
        /* Allocation failed – free whatever was allocated and return 0 */
        mem_free(curr); mem_free(next); mem_free(conv);
        return (seq_t)0;
    }

    /* Initialise row 0 */
    curr[0] = (seq_t)1;
    size_t curr_len = 1;

    /* Build rows successively until we reach row r */
    for (idx_t step = 0; step < r; ++step) {
        size_t next_len = curr_len + 2;        /* length of row (k+1) */
        size_t conv_len = next_len;           /* convolution length */

        /* Zero the convolution buffer for this iteration */
        for (size_t i = 0; i < conv_len; ++i) conv[i] = 0;

        /* Compute convolution with [1,4,4] */
        for (size_t i = 0; i < curr_len; ++i) {
            seq_t v = curr[i];

            conv[i] += v;                                 /* coefficient 1 */
            if (i + 1 < conv_len) conv[i + 1] += mul_small(v, 4);
            if (i + 2 < conv_len) conv[i + 2] += mul_small(v, 4);
        }

        /* Build the next row from the convolution result */
        next[0] = conv[1];
        for (size_t j = 1; j + 1 < next_len; ++j) {
            next[j] = conv[j + 1];
        }
        next[next_len - 1] = conv[1];                     /* last element */

        /* Prepare for next iteration */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;
        curr_len = next_len;
    }

    /* curr now points to row r; obtain the required element */
    seq_t result = curr[pos];

    /* Free allocated buffers */
    mem_free(curr);
    mem_free(next);
    mem_free(conv);

    return result;
}
