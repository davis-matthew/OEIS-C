/*
 * A086301  Numbers n such that 1 +/- 2n + 3n^2 are primes.
 * https://oeis.org/A086301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 311, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   isPrimeGeneric, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A086301: Numbers n such that 1 ± 2n + 3n^2 are both prime.
    The function returns the n‑th such integer, indexed with offset 1. */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_realloc.h"

/* Local cache of the sequence values */
static seq_t *cache          = NULL;   /* array holding a(i) values       */
static size_t cache_size     = 0;      /* number of elements stored       */
static size_t cache_cap      = 0;      /* current allocated capacity      */
static idx_t last_checked    = 0;      /* last m that was examined        */

/* Appends a value to the cache, expanding the array if necessary. */
static void append_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Returns the n‑th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; return 0 for 0 if called */
        return (seq_t)0;

    /* Use cached value if already known. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Search for the next qualifying integers starting after the last one we
       examined previously.  We update last_checked before any test so that
       successive calls do not re‑examine the same integer. */
    for (idx_t m = last_checked + 1; ; ++m) {
        last_checked = m;                                 /* update */

        seq_t mm    = (seq_t)m * (seq_t)m;                 /* m²          */
        seq_t plus  = 3 * mm + 2 * (seq_t)m + 1;          /* 3m²+2m+1    */
        seq_t minus = 3 * mm - 2 * (seq_t)m + 1;          /* 3m²-2m+1    */

        if (isPrimeGeneric(plus) && isPrimeGeneric(minus)) {
            append_cache((seq_t)m);
            if ((size_t)n == cache_size)               /* found it   */
                return cache[n - 1];
        }
    }
}
