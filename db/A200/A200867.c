/*
 * A200867  Number of 0..4 arrays x(0..n+1) of n+2 elements without any
 *          interior element greater than both neighbors or less than both
 *          neighbors.
 * https://oeis.org/A200867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + a(n-3) + 4*a(n-4) + a(n-6) + a(n-7)
 *            Initial values: a(1)=65, a(2)=195, a(3)=567, a(4)=1673,
 *            a(5)=4917, a(6)=14455, a(7)=42479
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Static cache for memoisation */
static seq_t *memo = NULL;    /* array indexed from 0 to n (1 unused) */
static idx_t memo_len = 0;    /* current allocated length */

/* Constants for the first 7 terms */
static const seq_t init_vals[7] = {
    (seq_t)65, (seq_t)195, (seq_t)567, (seq_t)1673,
    (seq_t)4917, (seq_t)14455, (seq_t)42479
};

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1 */

    /* Expand the cache if necessary */
    if (n >= memo_len) {
        size_t new_len = (size_t)(n + (idx_t)1);   /* include index 0 */
        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        memo = tmp;

        /* Ensure index 0 exists (unused) */
        if (memo_len == (idx_t)0) memo_len = (idx_t)1;

        /* Compute missing values up to n */
        for (idx_t i = memo_len; i <= n; ++i) {
            if (i <= (idx_t)7) {
                memo[i] = init_vals[i - (idx_t)1];
            } else {
                memo[i] = 3 * memo[i - (idx_t)1]
                          - memo[i - (idx_t)2]
                          + memo[i - (idx_t)3]
                          + 4 * memo[i - (idx_t)4]
                          + memo[i - (idx_t)6]
                          + memo[i - (idx_t)7];
            }
        }
        memo_len = n + (idx_t)1;
    }

    return memo[n];
}
