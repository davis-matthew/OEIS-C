/*
 * A208254  Number of n X 4 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward neighbors.
 * https://oeis.org/A208254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208254
   Recurrence:
     a(1)=14, a(2)=768, a(3)=43344, a(4)=2445264, a(5)=137947632
     a(n) = 61*a(n-1) - 266*a(n-2) + 416*a(n-3) - 256*a(n-4)   (n>5)
   The function caches computed values so that calls in any order are
   answered in O(1) after the initial work for the largest n seen. */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed values, indexed 1.. */
static idx_t allocated = 0;     /* maximum index for which memory is allocated */
static idx_t cached_max = 0;    /* maximum index for which a value is computed */

/* Ensure that cache has capacity for indices 0..n (i.e. size n+1). */
static void ensure_cache(idx_t n)
{
    if (n <= allocated)
        return;
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    /* zero‑initialize the new slots (not strictly needed for n<6 but harmless) */
    for (idx_t i = allocated + 1; i <= n; ++i)
        tmp[i] = 0;
    cache = tmp;
    allocated = n;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n<1, return 0 */

    /* Base values for n=1..5. */
    if (n <= 5) {
        static const seq_t base[] = {14, 768, 43344, 2445264, 137947632};
        return base[n - 1];
    }

    /* Return cached value if it already exists. */
    if (cached_max >= n)
        return cache[n];

    /* Make sure the first five values are stored. */
    if (cached_max < 5) {
        ensure_cache(5);
        static const seq_t base[] = {14, 768, 43344, 2445264, 137947632};
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = base[i - 1];
        cached_max = 5;
    }

    /* Compute up to n using the recurrence. */
    ensure_cache(n);
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        seq_t val = cache[i - 1];
        val = val * 61;
        val -= cache[i - 2] * 266;
        val += cache[i - 3] * 416;
        val -= cache[i - 4] * 256;
        cache[i] = val;
    }
    cached_max = n;
    return cache[n];
}
