/*
 * A199935  Size (b^3_n) of unit sphere in a certain graph (see Hazama article
 *          for precise definition).
 * https://oeis.org/A199935
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - a(n-5) with
 *            initial values: a(2)=0, a(3)=0, a(4)=2, a(5)=5, a(6)=9
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* size_t */
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

/* Cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static idx_t computed_upto = 1;/* highest index whose value is stored */

/* Ensure that the cache array can hold index 'n' (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)cache_cap > n)
        return;          /* already big enough */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr = seq_alloc_array(new_cap);
    if (!new_arr)          /* allocation failure – abort silently */
        return;

    if (cache) {
        /* copy existing contents */
        for (size_t i = 0; i < cache_cap; ++i)
            new_arr[i] = cache[i];
        free_seq_array(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 2; for n < 2 a harmless value (0) is returned. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;
    }

    /* make sure we have space for 'n' */
    ensure_cache_size(n);

    /* already computed? */
    if (n <= computed_upto) {
        return cache[n];
    }

    /* compute successive terms up to 'n' */
    for (idx_t k = computed_upto + 1; k <= n; ++k) {
        if (k == 2) {
            cache[2] = (seq_t)0;
        } else if (k == 3) {
            cache[3] = (seq_t)0;
        } else if (k == 4) {
            cache[4] = (seq_t)2;
        } else if (k == 5) {
            cache[5] = (seq_t)5;
        } else if (k == 6) {
            cache[6] = (seq_t)9;
        } else {
            seq_t a1 = cache[k-1];
            seq_t a2 = cache[k-2];
            seq_t a3 = cache[k-3];
            seq_t a4 = cache[k-4];
            seq_t a5 = cache[k-5];

            seq_t val = a1 * 3;
            val -= a2 * 3;
            val += a3;
            val += a4;
            val -= a5;

            cache[k] = val;
        }
    }

    computed_upto = n;
    return cache[n];
}
