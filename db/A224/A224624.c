/*
 * A224624  Number of (n+4) X 11 0..2 matrices with each 5 X 5 subblock
 *          idempotent.
 * https://oeis.org/A224624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 49, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224624
 *  Recurrence (empirical):
 *       a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4)
 *               + a(n-5) - 3*a(n-6) + 3*a(n-7) - a(n-8)   for n>12
 *  Initial values (a(1)…a(12)):
 *       54070, 31702, 36576, 39450, 41120, 42214,
 *       50748, 64133, 80045, 97220, 115074, 141044
 *
 *  Function a(idx_t n) returns the nth term (n ≥ 1).  The result is
 *  stored in a static cache so that calls in any order are fast.
 */

#include <stdlib.h>

static const seq_t init_vals[13] = {
    0ULL,           /* unused index 0 */
    54070ULL,       /* a(1) */
    31702ULL,       /* a(2) */
    36576ULL,       /* a(3) */
    39450ULL,       /* a(4) */
    41120ULL,       /* a(5) */
    42214ULL,       /* a(6) */
    50748ULL,       /* a(7) */
    64133ULL,       /* a(8) */
    80045ULL,       /* a(9) */
    97220ULL,       /* a(10) */
    115074ULL,      /* a(11) */
    141044ULL       /* a(12) */
};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but be tolerant */

    /* lazy initialization of cache with the first 12 terms */
    if (!cache) {
        cache_size = 13;              /* indices 0 … 12 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (size_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
        max_computed = 12;
    }

    /* if the requested term is already computed, return it */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size <= (size_t)n + 1)
            new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as a safe fallback */
            free(cache);
            cache = NULL;
            cache_size = 0;
            max_computed = 0;
            return 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute the terms from max_computed+1 up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k <= 12) {
            cache[(size_t)k] = init_vals[(size_t)k];
            continue;
        }
        /* recurrence using only non‑negative additions and a final subtraction */
        seq_t sum1 = 4 * cache[(size_t)(k - 1)];
        sum1 += 4 * cache[(size_t)(k - 3)];
        sum1 += cache[(size_t)(k - 5)];
        sum1 += 3 * cache[(size_t)(k - 7)];

        seq_t sum2 = 6 * cache[(size_t)(k - 2)];
        sum2 += cache[(size_t)(k - 4)];
        sum2 += 3 * cache[(size_t)(k - 6)];
        sum2 += cache[(size_t)(k - 8)];

        /* it is guaranteed that sum1 >= sum2 for the sequence */
        cache[(size_t)k] = sum1 - sum2;
    }

    max_computed = n;
    return cache[(size_t)n];
}
