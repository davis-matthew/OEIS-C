/*
 * A232943  Number of nX3 0..3 arrays with no element x(i,j) adjacent to value
 *          3-x(i,j) horizontally, vertically or antidiagonally, and top left
 *          element zero.
 * https://oeis.org/A232943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n−1) − 15*a(n−2) + 9*a(n−3) − 2*a(n−4) for n > 5
 *            Initial values: a(1)=9, a(2)=99, a(3)=1163, a(4)=13707,
 *            a(5)=161621
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache indexed 0.. */
static size_t cache_len = 0;      /* number of cached terms */

static const seq_t base_vals[6] = {0, 9, 99, 1163, 13707, 161621};

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* undefined for n=0, return 0 for safety */

    /* Ensure the cache contains at least n terms */
    if (cache_len < (size_t)n) {
        size_t new_len = (size_t)n;
        seq_t *new_buf = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!new_buf) abort();   /* out of memory */
        seq_cache = new_buf;

        /* Compute the missing terms */
        size_t i = cache_len + 1;
        for (; i <= new_len; ++i) {
            if (i <= 5) {
                seq_cache[i - 1] = base_vals[i];
            } else {
                /* recurrence: a(i) = 13*a(i-1) - 15*a(i-2) + 9*a(i-3) - 2*a(i-4) */
                seq_t term = 13 * seq_cache[i - 2];
                term -= 15 * seq_cache[i - 3];
                term +=  9 * seq_cache[i - 4];
                term -=  2 * seq_cache[i - 5];
                seq_cache[i - 1] = term;
            }
        }
        cache_len = new_len;
    }

    return seq_cache[n - 1];
}
