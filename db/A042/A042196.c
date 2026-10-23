/*
 * A042196  Numerators of continued fraction convergents to sqrt(623).
 * https://oeis.org/A042196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 1248*a(n-4) - a(n-8) initial values: a(0)=24, a(1)=25,
 *            a(2)=599, a(3)=624, a(4)=30551, a(5)=31175, a(6)=747576,
 *            a(7)=778751
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_computed = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Initialise cache and load the first eight terms */
    if (!initialized) {
        cache_cap = 16;          /* some room beyond the first 8 terms */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* abort on allocation failure */

        /* zero initialise */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = 0;

        cache[0] = (seq_t)24;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)599;
        cache[3] = (seq_t)624;
        cache[4] = (seq_t)30551;
        cache[5] = (seq_t)31175;
        cache[6] = (seq_t)747576;
        cache[7] = (seq_t)778751;

        last_computed = 7;
        initialized = true;
    }

    /* Ensure the cache can hold index n */
    if ((size_t)n > cache_cap - 1) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If the requested term is already computed, return it */
    if (n <= last_computed) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i < 8) continue;   /* first eight terms are already set */
        seq_t term = (seq_t)1248 * cache[i - 4] - cache[i - 8];
        cache[i] = term;
    }
    last_computed = n;
    return cache[n];
}
