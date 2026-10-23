/*
 * A037027  Skew Fibonacci-Pascal triangle read by rows.
 * https://oeis.org/A037027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 123, u32 n <= 413, u64 n <= 1499,
 *            u128 n <= 5493, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A037027: Skew Fibonacci‑Pascal triangle read by rows.
   T(n,m) = T(n-1,m) + T(n-2,m) + T(n-1,m-1)  for n≥m≥1,
   T(n,0) = Fibonacci(n+1),  T(n,m) = 0 if m>n.
   a(n) is the flattening of T(n,m) row by row (row 0 first, then row 1, …).
   The function below builds rows up to the one containing n and
   returns the appropriate entry.  Complexity O(r²) with r≈√(2n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r that contains index n.  The number of
       entries up to row r is (r+1)(r+2)/2.  */
    idx_t r = 0;
    while ( ( (r + 1) * (r + 2) ) / 2 <= n ) r++;

    /* The maximum row length we need.  Row r has r+1 elements. */
    size_t sz = (size_t)r + 1;

    /* Allocate three rows.  Cast from void* to seq_t* (required in C++). */
    seq_t *row0 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *row1 = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *row2 = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!row0 || !row1 || !row2) {
        free(row0); free(row1); free(row2);
        return 0;                /* OOM → return 0 as safe default */
    }

    /* Fibonacci values for T(i,0).  We maintain Fib(n) and Fib(n+1). */
    seq_t fib_prev = 0;   /* Fib(0)   */
    seq_t fib_cur  = 1;   /* Fib(1)   */

    /* Row 0 (i = 0).  T(0,0) = Fib(1) = 1. */
    row1[0] = fib_cur;
    if (n == 0) {
        seq_t res = row1[0];
        free(row0); free(row1); free(row2);
        return res;
    }

    /* Pointers to previous rows: prev2 = row i-2, prev1 = row i-1, cur = row i. */
    seq_t *prev2 = row0;
    seq_t *prev1 = row1;
    seq_t *cur   = row2;

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        /* Compute the Fibonacci number for this row: T(i,0). */
        seq_t next_fib = fib_prev + fib_cur;
        fib_prev = fib_cur;
        fib_cur  = next_fib;

        cur[0] = fib_cur;               /* T(i,0) */

        size_t ii = (size_t)i;          /* length of row i is i+1, indices 0…i */
        size_t len_prev1 = ii;          /* row i-1 has i elements (indices 0…i-1) */
        size_t len_prev2 = (i > 0 ? ii - 1 : 0); /* row i-2 has i-1 elements */

        for (size_t j = 1; j <= ii; ++j) {
            seq_t a_val = (j < len_prev1) ? prev1[j] : 0;
            seq_t b_val = (j < len_prev2) ? prev2[j] : 0;
            seq_t c_val = prev1[j - 1];
            cur[j] = a_val + b_val + c_val;
        }

        /* Does this row contain the desired index? */
        idx_t start = i * (i + 1) / 2;   /* first index of row i */
        idx_t end   = start + i;        /* last  index of row i */
        if (n >= start && n <= end) {
            seq_t result = cur[n - start];
            free(row0); free(row1); free(row2);
            return result;
        }

        /* Rotate row pointers for the next iteration. */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    /* The loop should always return above.  Fallback. */
    free(row0); free(row1); free(row2);
    return 0;
}
