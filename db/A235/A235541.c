/*
 * A235541  Number of (n+1) X (1+1) 0..1 arrays with the sum of each 2 X 2
 *          subblock two median terms lexicographically nondecreasing rowwise
 *          and columnwise.
 * https://oeis.org/A235541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A235541: number of (n+1)×2 binary arrays with specified
 * lexicographic constraints.
 *
 * Recurrence (valid for n ≥ 8, 1‐based indexing):
 *   a(n) = 4*a(n-1) - 14*a(n-3) + 5*a(n-4) + 18*a(n-5) - 4*a(n-6) - 8*a(n-7),
 * with initial terms:
 *   a(1)=16, a(2)=48, a(3)=141, a(4)=378, a(5)=988,
 *   a(6)=2482, a(7)=6109.
 *
 * The implementation memoises computed values in a static array.  The
 * array is resized only when needed.  All arithmetic is performed on
 * seq_t; intermediate sums are arranged to avoid unsigned underflow.
 *
 * The routine works for any seq_t provided; an arbitrary‑precision
 * implementation will correctly handle large values, while the 64‑bit
 * variant will return correct terms that fit in uint64_t.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* memoised sequence values (1‑based) */
static size_t cache_cap = 0;         /* current allocated capacity          */
static int cache_initialised = 0;    /* 1 if initial 7 terms are stored    */

/* Ensure the cache can address indices up to 'need' (inclusive). */
static void ensure_cache_size(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;

    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < (size_t)need + 1)
        new_cap <<= 1;                     /* power of two bigger than need+1 */

#if defined(__cplusplus)
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
    cache = new_cache;
#else
    seq_t *new_cache = realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort with undefined state */
        /* In a production environment you would handle this error. */
        return;
    }
    cache = new_cache;
#endif
    cache_cap = new_cap;
}

/* Initialise cache with the first seven terms of the sequence. */
static void initialise_cache(void)
{
    if (cache_initialised)
        return;
    ensure_cache_size(7);
    cache[1] = 16;
    cache[2] = 48;
    cache[3] = 141;
    cache[4] = 378;
    cache[5] = 988;
    cache[6] = 2482;
    cache[7] = 6109;
    cache_initialised = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* out of range – undefined in OEIS */

    initialise_cache();

    if (n <= 7)
        return cache[n];

    ensure_cache_size(n);

    /* Compute values iteratively up to n, storing them in the cache. */
    for (idx_t m = 8; m <= n; ++m) {
        /* Calculate positive and negative contributions separately to
           avoid unsigned underflow. */
        seq_t pos = cache[m-1] * 4
                  +  cache[m-4] * 5
                  +  cache[m-5] * 18;

        seq_t neg = cache[m-3] * 14
                  +  cache[m-6] * 4
                  +  cache[m-7] * 8;

        cache[m] = pos - neg;          /* pos ≥ neg for all n ≥ 8 */
    }
    return cache[n];
}
