/*
 * A220709  Number of ways to reciprocally link elements of a 4 X n array
 *          either to themselves or to exactly two horizontal and antidiagonal
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-4) - a(n-5) + a(n-6) for n > 7,
 *            a(1)=1, a(2)=1, a(3)=5, a(4)=13, a(5)=37, a(6)=105, a(7)=298
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache that survives between calls */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_size = 0;        /* allocated size */
static idx_t  cache_last = 0;        /* highest index that has been computed */

/* initialise cache with the first 7 terms of the sequence */
static void cache_init(void)
{
    if (cache != NULL) return;
    cache_size = 8;                      /* capacity for indices 0..7 */
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (cache == NULL) exit(EXIT_FAILURE);
    cache[0] = 0;        /* unused, but keeps indices 1..7 correct */
    cache[1] = 1;
    cache[2] = 1;
    cache[3] = 5;
    cache[4] = 13;
    cache[5] = 37;
    cache[6] = 105;
    cache[7] = 298;
    cache_last = 7;
}

/* grow the allocation if needed */
static void cache_ensure_alloc(idx_t n)
{
    if (cache == NULL) cache_init();
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_size = new_size;
    }
}

/* compute values up to index n using the recurrence */
static void cache_compute(idx_t n)
{
    if (cache_last >= n) return;
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t term = cache[i-1]            /* a(n-1) */
                  + (seq_t)6 * cache[i-2] /* + 6*a(n-2) */
                  - (seq_t)6 * cache[i-4] /* - 6*a(n-4) */
                  -           cache[i-5] /* - a(n-5) */
                  +           cache[i-6] /* + a(n-6) */;
        cache[i] = term;
    }
    cache_last = n;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset – harmless */
    cache_ensure_alloc(n);
    cache_compute(n);
    return cache[n];
}
