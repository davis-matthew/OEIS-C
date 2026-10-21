/*
 * A234107  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 12.
 * https://oeis.org/A234107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) - 50*a(n-3) + 16*a(n-4) + 80*a(n-5) -
 *            16*a(n-6) Offset: 1 (first term a(1) = 56)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init[6] = {56, 186, 604, 2064, 6964, 24156};
static seq_t *cache = NULL;      /* cache[0] holds a(1) */
static idx_t cache_len = 0;      /* number of elements currently cached */

/* Compute a(n), where n >= 1.  The function is memoized and can be called
   in any order. */
seq_t a(idx_t n)
{
    if (n <= 6) {
        /* terms before the recurrence */
        return init[n - 1];
    }

    /* initialise cache with first six terms if not already done */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – unlikely in tests */
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_len = 6;
    }

    /* extend cache up to n if necessary */
    if (n > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;

        /* compute remaining terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* indices: cache[i-2] = a(i-1), cache[i-3] = a(i-2), ... */
            seq_t t1 = cache[i - 2]; /* a(i-1) */
            seq_t t2 = cache[i - 3]; /* a(i-2) */
            seq_t t3 = cache[i - 4]; /* a(i-3) */
            seq_t t4 = cache[i - 5]; /* a(i-4) */
            seq_t t5 = cache[i - 6]; /* a(i-5) */
            seq_t t6 = cache[i - 7]; /* a(i-6) */

            /*   5*a(i-1) + 6*a(i-2) + 16*a(i-4) + 80*a(i-5)
                  - (50*a(i-3) + 16*a(i-6)) */
            seq_t sum = t1 * 5 + t2 * 6 + t4 * 16 + t5 * 80;
            seq_t sub = t3 * 50 + t6 * 16;
            cache[i - 1] = sum - sub;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
