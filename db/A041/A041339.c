/*
 * A041339  Denominators of continued fraction convergents to sqrt(183).
 * https://oeis.org/A041339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   OEIS A041339
 *   Denominators of continued fraction convergents to sqrt(183).
 *   Formula: a(n) = 974*a(n-6) - a(n-12)  for n >= 12
 *   initial terms:
 *     a(0)=1, a(1)=1, a(2)=2, a(3)=17, a(4)=19, a(5)=36,
 *     a(6)=955, a(7)=991, a(8)=1946, a(9)=16559, a(10)=18505, a(11)=35064
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed values          */
static size_t cache_size = 0;        /* allocated size of cache array            */
static idx_t  max_computed = 0;      /* largest n for which cache[n] is valid    */

/* initialise cache with the first 12 terms */
static void init_cache(void)
{
    static const seq_t init_vals[12] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)17, (seq_t)19, (seq_t)36,
        (seq_t)955, (seq_t)991, (seq_t)1946, (seq_t)16559,
        (seq_t)18505, (seq_t)35064
    };

    cache_size = 12;
    cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    if (!cache) return;                 /* allocation failure – unlikely in tests   */

    for (size_t i = 0; i < 12; ++i)
        cache[i] = init_vals[i];

    max_computed = 11;                  /* we have computed indices 0 .. 11          */
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* Ensure cache is initialised */
    if (!cache)
        init_cache();
    if (!cache)                 /* allocation failure: return 0 as safe default */
        return (seq_t)0;

    /* For n < 12 we already have the values */
    if (n < 12)
        return cache[(size_t)n];

    /* If requested value has already been computed, return it */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* Extend cache if necessary */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* allocation failure           */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute subsequent terms using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i >= 12, recurrence applies */
        cache[(size_t)i] = ((seq_t)974 * cache[(size_t)(i - 6)])
                           - cache[(size_t)(i - 12)];
    }

    max_computed = n;
    return cache[(size_t)n];
}
