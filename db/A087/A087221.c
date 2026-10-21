/*
 * A087221  Number of compositions (ordered partitions) of n into powers of 4.
 * https://oeis.org/A087221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k ≥ 0, 4^k ≤ n} a(n – 4^k) with a(0) = 1
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 70, u64 n <= 138,
 *            u128 n <= 274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a memoised table that grows
   lazily on demand.  It works for any integer type seq_t
   supplied by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* array of a(i)     */
    static size_t capacity = 0;         /* currently allocated cells */
    static idx_t limit = 0;             /* smallest index not yet computed */

    /* Ensure at least one slot for a(0) */
    if (capacity == 0) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;           /* allocation failed – return 0 */
        cache[0] = (seq_t)1;            /* a(0) = 1   */
        limit = 1;                      /* next index to fill */
    }

    /* If a(n) already known, just return it. */
    if (n < limit)
        return cache[n];

    /* Grow the array if required */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)(n + 1))
            new_cap = new_cap * 2 + 1;   /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute all values from limit up to n */
    for (idx_t i = limit; i <= n; ++i) {
        seq_t sum = 0;
        idx_t pow = 1;                   /* 4^0 */
        while (pow <= i) {
            sum += cache[i - pow];
            if (pow > i / 4)             /* avoid overflow of pow */
                break;
            pow *= 4;                    /* next power: 4^1, 4^2, ...   */
        }
        cache[i] = sum;
    }
    limit = n + 1;                       /* all entries up to n are now computed */

    return cache[n];
}
