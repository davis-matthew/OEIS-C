/*
 * A207389  Number of n X 6 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) * a(1)=21, a(2)=441, a(3)=2307,
 *            a(4)=7561, a(5)=19319, a(6)=42167, a(7)=82477
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 64, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Memoised recurrence implementation (O(n) with linear memory)        */
/* --------------------------------------------------------------------- */

static seq_t *cache      = NULL;     /* cache[1..max]  (index 0 unused) */
static size_t cache_cap  = 0;        /* allocated capacity (>= cache_len+1) */
static size_t cache_len  = 0;        /* highest computed index (0 if none) */

seq_t a(idx_t n)
{
    /* Defensive: offset starts at 1. For n<1 return 0 (unused). */
    if (n == 0) return 0;

    /* -----------------------------------------------------------------
     * Initialise cache with the first 7 terms and minimal capacity.
     * --------------------------------------------------------------- */
    if (cache_cap == 0) {
        cache_cap = 8;                                 /* 1 dummy + 1..7 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        /* Initial terms (index 1..7) */
        cache[1] = 21;
        cache[2] = 441;
        cache[3] = 2307;
        cache[4] = 7561;
        cache[5] = 19319;
        cache[6] = 42167;
        cache[7] = 82477;

        cache_len = 7;
    }

    /* -----------------------------------------------------------------
     * Grow the storage if the requested index exceeds the current capacity.
     * --------------------------------------------------------------- */
    if ((size_t)n > cache_cap - 1) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* -----------------------------------------------------------------
     * Compute all missing terms up to n using the recurrence.
     * --------------------------------------------------------------- */
    if ((size_t)n > cache_len) {
        for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
            /* Perform the recurrence with seq_t arithmetic.  All intermediate
             * operations stay within seq_t because the recurrence has only
             * additions and multiplications by small constants. */
            cache[k] =
                  7 * cache[k-1]
                - 21 * cache[k-2]
                + 35 * cache[k-3]
                - 35 * cache[k-4]
                + 21 * cache[k-5]
                -  7 * cache[k-6]
                +  cache[k-7];
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
