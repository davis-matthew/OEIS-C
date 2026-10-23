/*
 * A127273  Numbers n such that sum of the digits of n and of n+1 divides n +
 *          (n+1), n >= 1.
 * https://oeis.org/A127273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A127273: numbers n such that sum_of_digits(n)+sum_of_digits(n+1)
   divides n+(n+1). This function searches integers in increasing order,
   caching the found terms.  */

#include <stddef.h>                   /* size_t, NULL */
#include "../utils/ensure_capacity.h" /* ensure_capacity() */

/* Helper: sum of decimal digits of an idx_t value. */
static idx_t digit_sum_idx(idx_t x) {
    idx_t s = 0;
    while (x > 0) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of cached terms          */
    static size_t cap  = 0;   /* allocated capacity of cache     */
    static idx_t  next = 1;   /* next integer to test (1‑based)   */

    /* Grow the cache until it contains at least n terms */
    while (sz < n) {
        idx_t sum1 = digit_sum_idx(next);
        idx_t sum2 = digit_sum_idx(next + 1);
        idx_t tot  = next + (next + 1);      /* 2*next + 1                 */
        idx_t totSum = sum1 + sum2;

        if (tot % totSum == 0) {
            /* Ensure capacity for the new element */
            ensure_capacity(sz + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[sz++] = (seq_t)next;   /* cast to seq_t – ok for fitting values */
        }
        ++next;
    }

    return cache[n - 1];
}
