/*
 * A173905  {2,3} and the nonprimes A141468.
 * https://oeis.org/A173905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are not prime except the constants 2 and 3.
   All nonnegative integers ≤3, and all composite integers ≥4. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* stored terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static size_t cache_count = 0;       /* number of terms currently stored */
static idx_t last_checked = 0;       /* next integer to examine */

/* Ensure the cache has space for at least `need` terms. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms until the cache contains `need` entries. */
static void fill_cache(size_t need)
{
    while (cache_count < need) {
        if (last_checked <= 3 || !is_prime(last_checked))
            cache[cache_count++] = (seq_t)last_checked;
        ++last_checked;
    }
}

/* External interface. */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n<1 – return 0 safely */
        return (seq_t)0;

    grow_cache((size_t)n);
    fill_cache((size_t)n);
    return cache[(size_t)n - 1];
}
