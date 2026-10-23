/*
 * A152055  a(n) = ((8 + sqrt(3))^n + (8 - sqrt(3))^n)/2.
 * https://oeis.org/A152055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8 + sqrt(3))^n + (8 - sqrt(3))^n)/2 Recurrence: a(0) =
 *            1, a(1) = 8 a(n) = 16*a(n-1) - 61*a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;          /* need space for 0 .. n */
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, we just leave cache as is; the program would
       terminate anyway – acceptable for this simple implementation. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using the recurrence with memoisation */
seq_t a(idx_t n)
{
    /* Ensure cache is large enough and initialise if necessary */
    if (cache_size == 0) {
        ensure_cache(1);          /* create space for indices 0 and 1 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache_size = 2;
    }

    /* Extend cache up to n if needed */
    if (cache_size <= n) {
        ensure_cache(n);
        /* Compute missing terms */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t a_nm1 = cache[i - 1];
            seq_t a_nm2 = cache[i - 2];
            cache[i] = (seq_t)( (seq_t)16 * a_nm1 - (seq_t)61 * a_nm2 );
        }
        cache_size = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
