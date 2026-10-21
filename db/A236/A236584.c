/*
 * A236584  The number of tilings of a 9 X (2n) floor with 2 X 3 hexominoes.
 * https://oeis.org/A236584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 4*a(n-3) - 2*a(n-4) with initial
 *            values a(0)=1, a(1)=1, a(2)=1, a(3)=5
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache to enable fast repeated calls with arbitrary argument order */
static seq_t *cache = NULL;          /* Cached sequence values */
static size_t cache_len = 0;         /* Number of elements that have been computed (≤ cache_cap) */
static size_t cache_cap = 0;         /* Current allocation size */

/* Helper to fill the initial four terms */
static void init_cache(void)
{
    if (cache_len < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)5;
        cache_len = 4;
    }
}

seq_t a(idx_t n)
{
    /* Handle the trivial case first */
    if (n < 4) {
        static const seq_t base[4] = {(seq_t)1, (seq_t)1, (seq_t)1, (seq_t)5};
        return base[(size_t)n];
    }

    /* Ensure we have an array large enough to hold indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        /* Grow by at least doubling strategy to keep amortized cost O(1) */
        size_t new_cap = needed * 2;
        if (new_cap < needed) new_cap = needed; /* protect against overflow */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed; fallback to computing the value directly
               (should never happen for the given constraints) */
            seq_t val = 0;
            return val;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first four terms if needed */
    init_cache();

    /* Compute new terms up to n if not already computed */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Recurrence: a(i) = 2*a(i-1) - a(i-2) + 4*a(i-3) - 2*a(i-4) */
        cache[i] = 2 * cache[i-1] - cache[i-2] + 4 * cache[i-3] - 2 * cache[i-4];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
