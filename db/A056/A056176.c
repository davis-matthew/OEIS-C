/*
 * A056176  Sum of a(n) terms of 1/k^(1/4) first exceeds n.
 * https://oeis.org/A056176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = min{ m ≥ 1 | Σ_{k=1}^{m} 1/k^(1/4) > n } */
#include <stdlib.h>
#include <math.h>

/* Dynamic array of cumulative sums: cum_sum[i] = Σ_{k=1}^{i} 1/k^(1/4),
   with cum_sum[0] = 0.  The array grows as needed. */
static long double *cum_sum = NULL;
static size_t cum_len = 0;     /* number of stored values, ≥ 1 */
static size_t cum_cap = 0;     /* current allocated capacity */

/* Ensure the cached sums exceed the requested n. */
static void ensure_cache(idx_t n)
{
    if (cum_len == 0) {
        cum_cap = 1024;
        cum_sum = (long double *)malloc(cum_cap * sizeof(long double));
        cum_sum[0] = 0.0L;       /* sum for k = 0 */
        cum_len = 1;
    }
    while (cum_sum[cum_len - 1] <= (long double)n) {
        if (cum_len == cum_cap) {
            cum_cap *= 2;
            cum_sum = (long double *)realloc(cum_sum, cum_cap * sizeof(long double));
        }
        idx_t k = cum_len;                     /* next term (1‑based) */
        long double term = powl((long double)k, -0.25L); /* 1 / k^(1/4) */
        cum_sum[cum_len] = cum_sum[cum_len - 1] + term;
        ++cum_len;
    }
}

/* Return a(n). */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Binary search for the smallest index m with cum_sum[m] > n. */
    idx_t lo = 1;
    idx_t hi = (idx_t)(cum_len - 1);
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cum_sum[mid] <= (long double)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;
}
