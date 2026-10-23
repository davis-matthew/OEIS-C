/*
 * A041655  Denominators of continued fraction convergents to sqrt(346).
 * https://oeis.org/A041655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 84,
 *            big any n
 * Helpers:   alloc_cache, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 186*a(n-5) + a(n-10) for n>9
   Initial values:
   a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5,
   a(5)=183, a(6)=188, a(7)=371, a(8)=559, a(9)=930
*/
#include <stdlib.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_seq.h"

static const seq_t init_vals[10] = {
    (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5,
    (seq_t)183, (seq_t)188, (seq_t)371, (seq_t)559, (seq_t)930
};

static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of values stored (≥0) */

/*
 * Ensure the cache can hold `needed` elements (needed >= cache_cap).
 * Allocates a new array (using the provided alloc_cache helper),
 * copies existing values, frees the old array, and updates pointers.
 */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_cache = alloc_cache(new_cap);
    if (!new_cache)
        return;   /* allocation failure – silent; caller may get garbage */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache)
        free_seq(cache);

    cache = new_cache;
    cache_cap = new_cap;
}

/*
 * Return the n-th term of A041655.
 * Works for arbitrary order of calls and for any n ≥ 0.
 */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* idx_t is unsigned 64‑bit */
    ensure_cache_size(idx + 1);      /* need space for element n */

    /* initialise base cases if not already done */
    if (cache_len == 0) {
        size_t upto = idx < 10 ? idx : 9;
        for (size_t i = 0; i <= upto; ++i)
            cache[i] = init_vals[i];
        cache_len = upto + 1;
    }

    /* compute all missing values up to idx */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 10) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = (seq_t)186 * cache[i - 5] + cache[i - 10];
        }
    }
    cache_len = idx + 1;

    return cache[idx];
}
