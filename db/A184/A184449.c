/*
 * A184449  1/36 the number of (n+2) X 3 0..2 arrays with each 3 X 3 subblock
 *          containing two of one value, two of another, and five of the last.
 * https://oeis.org/A184449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1..7) given below; for n >= 8
   a(n) = 3*a(n-1) + 162*a(n-3) - 486*a(n-4)
          - 5832*a(n-6) + 17496*a(n-7) */
#include <stdlib.h>

static const seq_t _a_init[7] = {
    (seq_t)63,    (seq_t)234,   (seq_t)864,
    (seq_t)3159,  (seq_t)12879, (seq_t)51759,
    (seq_t)203391
};

seq_t a(idx_t n)
{
    /* a(0) is unused; if called with 0, return 0 */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;   /* cache[1..] holds the terms */
    static idx_t cached = 0;      /* size of allocated cache (max index + 1) */

    /* Allocate (or enlarge) the cache to at least n+1 elements */
    if (cached <= n) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed – unlikely in tests */
        cache = tmp;

        /* Ensure initial values are set */
        for (idx_t i = 1; i <= 7 && i < newsize; ++i)
            cache[i] = _a_init[i - 1];

        /* Compute missing terms */
        idx_t start = cached;
        if (start < 8) start = 8;        /* recurrence only defined for n ≥ 8 */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1]
                     + 162 * cache[i - 3]
                     - 486 * cache[i - 4]
                     - 5832 * cache[i - 6]
                     + 17496 * cache[i - 7];
        }
        cached = newsize;
    }

    return cache[n];
}
