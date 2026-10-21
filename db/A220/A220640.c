/*
 * A220640  Number of ways to reciprocally link elements of an n X 4 array
 *          either to themselves or to exactly one king-move neighbor.
 * https://oeis.org/A220640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) +114*a(n-2) -325*a(n-3) -340*a(n-4) +460*a(n-5)
 *            +245*a(n-6) -48*a(n-7) +2*a(n-8) -15*a(n-9) initial values:
 *            a(0)=1, a(1)=5, a(2)=172, a(3)=3755, a(4)=92801, a(5)=2226936,
 *            a(6)=53841725, a(7)=1299348473, a(8)=31371388772
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Cached values of the sequence */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements already in cache */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure cache contains at least n+1 elements by reallocating if necessary */
static void grow_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;  /* double until enough */

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL) abort();   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth element of sequence A220640 */
seq_t a(idx_t n)
{
    if (n < cache_size)
        return cache[n];          /* already computed */

    grow_cache(n);

    /* Values defined by the OEIS entry for indices 0..8 */
    static const seq_t init[9] = {
        (seq_t) 1ULL,
        (seq_t) 5ULL,
        (seq_t)172ULL,
        (seq_t)3755ULL,
        (seq_t)92801ULL,
        (seq_t)2226936ULL,
        (seq_t)53841725ULL,
        (seq_t)1299348473ULL,
        (seq_t)31371388772ULL
    };

    /* Compute missing values */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = init[i];
        } else {
            /* Use the 9‑term linear recurrence */
            seq_t v = cache[i-1] * (seq_t)20;
            v += cache[i-2] * (seq_t)114;
            v -= cache[i-3] * (seq_t)325;
            v -= cache[i-4] * (seq_t)340;
            v += cache[i-5] * (seq_t)460;
            v += cache[i-6] * (seq_t)245;
            v -= cache[i-7] * (seq_t)48;
            v += cache[i-8] * (seq_t)2;
            v -= cache[i-9] * (seq_t)15;
            cache[i] = v;
        }
    }

    cache_size = n + 1;
    return cache[n];
}
