/*
 * A202428  Number of (n+2) X 3 binary arrays avoiding patterns 000 and 001 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
   a(1) = 108
   a(2) = 390
   a(3) = 1344
   a(4) = 4680
   a(5) = 16254
   a(6) = 56508
   a(n) = a(n-1) + 7*a(n-2) + 6*a(n-3) - a(n-4) - 2*a(n-5) + a(n-6)   (n ≥ 7) */

#include <stdlib.h>

static seq_t *cache = NULL;     /* index 0 unused; cache[n] holds a(n) */
static idx_t  cache_sz = 0;     /* highest n whose value is cached */

/* Extend the cache up to at least n and compute missing terms. */
static void compute_up_to(idx_t n)
{
    if (n <= cache_sz)
        return;

    /* Reallocate to new size */
    idx_t new_sz = n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_sz + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();              /* out of memory */
    cache = new_cache;

    /* Initialise new entries to zero */
    for (idx_t i = cache_sz + 1; i <= new_sz; ++i)
        cache[i] = 0;

    /* Ensure the first six base terms are set */
    if (cache[1] == 0 && new_sz >= 1) cache[1] = 108;
    if (cache[2] == 0 && new_sz >= 2) cache[2] = 390;
    if (cache[3] == 0 && new_sz >= 3) cache[3] = 1344;
    if (cache[4] == 0 && new_sz >= 4) cache[4] = 4680;
    if (cache[5] == 0 && new_sz >= 5) cache[5] = 16254;
    if (cache[6] == 0 && new_sz >= 6) cache[6] = 56508;

    /* Compute values using the recurrence, starting at index 7 */
    idx_t start = cache_sz + 1;
    if (start < 7)
        start = 7;
    for (idx_t i = start; i <= new_sz; ++i) {
        cache[i] = cache[i-1]
                 + 7 * cache[i-2]
                 + 6 * cache[i-3]
                 -   cache[i-4]
                 - 2 * cache[i-5]
                 +   cache[i-6];
    }

    cache_sz = new_sz;
}

/* Public function required by the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined offset, return 0 */
    compute_up_to(n);
    return cache[n];
}
