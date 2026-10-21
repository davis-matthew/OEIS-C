/*
 * A218065  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..1 2 X n array.
 * https://oeis.org/A218065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 4*a(n-2) + a(n-3) – 3*a(n-4) – 2*a(n-5), a(1)=4,
 *            a(2)=6, a(3)=18, a(4)=42, a(5)=108
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

/* Static cache of already computed values, 1‑based storage at cache[n‑1] */
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   /* number of computed values */
static size_t cache_cap   = 0;   /* allocated capacity */

/* Compute all terms up to n (1‑based).  Thread‑safe code is not required. */
static void compute_to(idx_t n)
{
    if (n == 0) return;

    /* Ensure capacity: always keep at least 5 slots for the recurrence. */
    size_t target   = (size_t)n;                /* fits because n ≤ 10 000 in tests */
    size_t new_cap  = (target > 5 ? target : 5);/* at least five entries          */

    if (new_cap > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – keep cache empty; caller will see 0. */
            free(cache);
            cache = NULL;
            cache_len = 0;
            cache_cap = 0;
            return;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first five terms once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)18;
        cache[3] = (seq_t)42;
        cache[4] = (seq_t)108;
        cache_len = 5 > cache_cap ? cache_cap : 5;
    }

    /* Compute missing terms up to 'target'. */
    for (size_t i = cache_len + 1; i <= target; ++i) {
        if (i <= 5) {
            /* Already initialised – nothing to do. */
            continue;
        }
        seq_t left  = cache[i - 2] + cache[i - 4] + cache[i - 3] * (seq_t)4;
        seq_t right = cache[i - 5] * (seq_t)3 + cache[i - 6] * (seq_t)2;
        cache[i - 1] = left - right;
    }
    cache_len = target;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 – return 0 */
    compute_to(n);
    return cache[(size_t)n - 1];
}
