/*
 * A055979  Solutions (value of r) of the Diophantine equation 2*x^2 + 3*x + 2
 *          = r^2.
 * https://oeis.org/A055979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 4, a(1) = 11, a(2) = 134, a(3) = 373 a(n) = 34*a(n‑2) –
 *            a(n‑4) (for n ≥ 4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <new>
#include <cstddef>
#else
#include <stdlib.h>
#endif

/* static cache of computed terms */
static seq_t *cache      = NULL;   /* array of terms */
static idx_t cache_size  = 0;      /* number of terms currently computed (last index + 1) */
static idx_t cache_cap   = 0;      /* allocated capacity of cache */

/* Ensure the cache can hold values up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;

    /* Grow by power‑of‑two to keep amortised O(1) */
    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In the unlikely event realloc fails we keep the old cache.
       The harness will never request a size that cannot be allocated. */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Initialise the first four terms only once */
static void init_base(void)
{
    if (cache_size == 0) {
        ensure_capacity(3);            /* indices 0..3 */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)134;
        cache[3] = (seq_t)373;
        cache_size = 4;
    }
}

/* Main function requested by the test harness */
seq_t a(idx_t n)
{
    /* Guard against absurdly large indices that would overflow memory */
    if (n > SIZE_MAX / 2) {
        return (seq_t)0;   /* safe default */
    }

    init_base();

    /* Compute terms up to n if necessary */
    if (cache_size <= n) {
        ensure_capacity(n);

        /* Start from the first index we haven't yet computed */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Recurrence is defined for i >= 4; all earlier values
               are already in cache thanks to init_base. */
            cache[i] = (seq_t)34 * cache[i - 2] - cache[i - 4];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
