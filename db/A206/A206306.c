/*
 * A206306  Riordan array (1, x/(1-3*x+2*x^2)).
 * https://oeis.org/A206306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 69, u32 n <= 236, u64 n <= 791,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 71 terms through n = 1306 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A206306: Convolution triangle of the Mersenne numbers.
 * Triangle T(n,k) read by rows; the sequence a(n) is the flattened rows.
 * Recurrence:  T(n,k) = 3*T(n-1,k) + T(n-1,k-1) - 2*T(n-2,k)   for 1 ≤ k < n
 * with base T(0,0)=1, T(n,0)=0 (n>0), T(n,n)=1.
 * a(n) is obtained by mapping n to the pair (row,col) with row = floor((√(8n+1)-1)/2)
 * and col = n - row*(row+1)/2.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Find the largest row r such that r*(r+1)/2 <= n. */
static idx_t highest_row(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while (hi * (hi + 1) / 2 <= n) {
        hi <<= 1;          /* double hi until it overshoots */
    }
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Special case: n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Determine the row and column in the triangle that corresponds to index n. */
    idx_t row = highest_row(n);
    idx_t col = n - row * (row + 1) / 2;

    /* Allocate rows: we only need the previous two rows at any time. */
    seq_t *prev_prev = mem_new(seq_t, 1);   /* row r-2 */
    seq_t *prev      = mem_new(seq_t, 2); /* row r-1 */

    /* row 0 */
    prev_prev[0] = (seq_t)1;      /* T(0,0) */

    if (row == 0) {               /* we already handled n==0 above */
        mem_free(prev_prev);
        mem_free(prev);
        return (seq_t)1;
    }

    /* row 1 */
    prev[0] = (seq_t)0;           /* T(1,0) */
    prev[1] = (seq_t)1;           /* T(1,1) */

    if (row == 1)
    {
        seq_t result = prev[col];
        mem_free(prev_prev);
        mem_free(prev);
        return result;
    }

    /* Compute rows 2 .. row */
    for (idx_t r = 2; r <= row; ++r)
    {
        seq_t *cur = mem_new(seq_t, (r + 1));

        /* First column (k = 0) is always 0 for n > 0. */
        cur[0] = (seq_t)0;

        /* Middle columns (1 <= k <= r-1) use the recurrence. */
        for (idx_t k = 1; k <= r - 1; ++k)
        {
            seq_t a_val = 0;  /* T(r-1,k) */
            if (k <= r - 1) /* prev has size r */
                a_val = prev[k];

            seq_t b_val = 0;  /* T(r-1,k-1) */
            if (k > 0)
                b_val = prev[k - 1];

            seq_t c_val = 0;  /* T(r-2,k) */
            if (k <= r - 2)  /* prev_prev has size r-1 */
                c_val = prev_prev[k];

            /* Compute 3*a_val + b_val - 2*c_val; the result is non‑negative. */
            seq_t temp = (seq_t)3 * a_val + b_val;
            cur[k] = temp - (seq_t)2 * c_val;
        }

        /* Last column (k = r) is always 1. */
        cur[r] = (seq_t)1;

        /* Rotate the row pointers for next iteration. */
        mem_free(prev_prev);
        prev_prev = prev;
        prev = cur;
    }

    /* Now 'prev' holds the desired row. */
    seq_t result = prev[col];

    mem_free(prev_prev);
    mem_free(prev);

    return result;
}
