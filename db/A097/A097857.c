/*
 * A097857  a(0) = 1, a(1) = 1, a(n) = floor((a(n-1)+a(n-2))/2) + 3 if a(n-2)
 *          is even, a(n) = floor((a(n-1)-a(n-2))/2) + 3 if a(n-2) is odd.
 * https://oeis.org/A097857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(1) = 1; a(n) = floor((a(n-1)+a(n-2))/2) + 3 if
 *            a(n-2) is even, a(n) = floor((a(n-1)-a(n-2))/2) + 3 if a(n-2) is
 *            odd
 * Range:     i8 n <= 113, i16 n <= 113, i32 n <= 113, i64 n <= 113,
 *            i128 n <= 113, big any n
 * Validated: partial, pass, 114 terms through n = 113 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: <stdlib.h>              (malloc, realloc, abort)
   Works for both C11 and C++17.          */

#include <stdlib.h>
#include "../utils/mem.h"

/* Floor division by 2 that works for negative seq_t values */
static inline seq_t floor_div2(seq_t x)
{
    if (x >= 0) return x / 2;
    /* x is negative: floor division is the same as truncation toward
       negative infinity. In C and C++ integer division truncates toward
       zero, so we adjust.  */
    return -(((-x) + 1) / 2);
}

/* Static cache to allow non‑sequential calls. */
static seq_t *memo = NULL;
static idx_t memo_len = 0;

/* The recurrence implementation. */
seq_t a(idx_t n)
{
    /* Initialise the first two terms lazily. */
    if (memo_len == 0) {
        memo_len = 2;
        memo = mem_new(seq_t, memo_len);
        if (!memo) abort();
        memo[0] = 1;
        memo[1] = 1;
    }

    /* Serve cached values. */
    if (n < memo_len) return memo[n];

    /* Grow the cache to include index n. */
    idx_t required = n + 1;
    if (required > memo_len) {
        seq_t *tmp = mem_resize(seq_t, memo, required);
        if (!tmp) abort();
        memo = tmp;

        for (idx_t i = memo_len; i <= n; ++i) {
            seq_t prev  = memo[i - 1];
            seq_t prev2 = memo[i - 2];
            seq_t result;

            if ((prev2 & 1) == 0) {            /* a(n-2) is even */
                result = floor_div2(prev + prev2) + 3;
            } else {                           /* a(n-2) is odd */
                result = floor_div2(prev - prev2) + 3;
            }
            memo[i] = result;
        }
        memo_len = required;
    }

    return memo[n];
}
