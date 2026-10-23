/*
 * A048437  Take the first n numbers written in base 5, concatenate them, then
 *          convert from base 5 to base 10.
 * https://oeis.org/A048437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate base-5 representations of 1..n, interpreted
 *            as a base-10 integer
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to compute powers of 5 up to a reasonable bound.
   These are used both as uint64 thresholds for digit counting
   and as seq_t values for the actual computation. */
static idx_t pow5_limit[32];
static seq_t pow5_seq[32];
static bool pow5_initialized = false;

static void init_pow5(void)
{
    if (pow5_initialized) return;
    pow5_limit[0] = 1;          /* 5^0 */
    pow5_seq[0]   = (seq_t)1;   /* seq_t version of 5^0 */
    for (int i = 1; i < 32; ++i) {
        pow5_limit[i] = pow5_limit[i-1] * (idx_t)5;
        pow5_seq[i]   = pow5_seq[i-1] * (seq_t)5;
    }
    pow5_initialized = true;
}

/* Cache of previously computed terms.
   cache[0] is unused (set to 0 for convenience). */
static seq_t *cache = NULL;      /* array of size cache_size + 1 */
static idx_t cache_size = 0;     /* maximum n that has been computed */

/* Computes a(n) for any n >= 1.  The function stores results
   in a static cache so that repeated calls with different n
   will reuse already computed values. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1; this case is not used by the OEIS specification */

    init_pow5();

    /* Ensure the cache holds terms up to n. */
    if (n > cache_size) {
        /* Grow the cache if necessary. */
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (!new_cache) abort();   /* memory allocation failure */
        cache = new_cache;

        /* Initialize cache[0] if needed. */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
        }

        /* Compute terms starting from cache_size + 1 up to n. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* Determine number of base-5 digits of i. */
            int digits = 1;
            while (i >= pow5_limit[digits]) ++digits;

            /* Update the running value: res = res * 5^digits + i. */
            seq_t prev = cache[i-1];
            seq_t mult = pow5_seq[digits];
            cache[i] = prev * mult + (seq_t)i;
        }
        cache_size = n;
    }

    return cache[n];
}
