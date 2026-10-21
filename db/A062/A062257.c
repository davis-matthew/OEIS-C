/*
 * A062257  Number of (0,1)-strings of length n with no occurrences of the
 *          substrings 10101101 and 1110101.
 * https://oeis.org/A062257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-5) + a(n-6) + a(n-8) + a(n-9) - a(n-10) (n
 *            ≥ 10) Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16,
 *            a(5)=32, a(6)=64, a(7)=127, a(8)=251, a(9)=496 (The recurrence
 *            uses only earlier terms, so the sequence is well-defined.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values */
static size_t cache_size = 0;    /* number of filled entries */
static size_t cache_cap  = 0;    /* allocated capacity */

/* initialise cache with first 10 terms */
static void init_cache(void)
{
    static const seq_t init[10] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
        (seq_t)32, (seq_t)64, (seq_t)127, (seq_t)251, (seq_t)496
    };

    if (cache_cap < 10) {
        if (cache) free(cache);          /* reinitialising, free old storage */
        cache_cap = 10;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
    cache_size = 10;
}

/* ensure that storage exists for indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_size) return;

    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (!cache) init_cache();              /* lazy initialisation */

    if (n >= cache_size) {
        ensure_capacity(n);
        /* compute missing entries */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i < 10) continue;         /* already initialised by init_cache */

            seq_t val = 2 * cache[i - 1];
            val -= cache[i - 5];
            val += cache[i - 6];
            val += cache[i - 8];
            val += cache[i - 9];
            val -= cache[i - 10];
            cache[i] = val;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
