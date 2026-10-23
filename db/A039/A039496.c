/*
 * A039496  Numbers whose base-12 representation has the same nonzero number
 *          of 0's and 4's.
 * https://oeis.org/A039496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑12 representation has
 *            a non‑zero number of 0's and a non‑zero number of 4's, with
 *            those two counts equal
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of found terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements allocated */
static idx_t term_count = 0;   /* number of terms stored in cache */
static seq_t last_checked = (seq_t)0;

/* helper: ensure cache has at least `need` capacity */
static void ensure_cache_capacity(idx_t need) {
    if (cache_size >= need)
        return;
    idx_t new_size = cache_size ? cache_size : 128;
    while (new_size < need)
        new_size <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */
    cache = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == 0)   /* offset starts at 1; undefined for n<1, but return 0 */
        return (seq_t)0;

    /* lazy initialization of cache */
    if (cache == NULL) {
        ensure_cache_capacity(128);
    }

    /* continue searching until we have at least n terms */
    while (term_count < n) {
        seq_t candidate = last_checked + (seq_t)1;
        seq_t temp = candidate;
        idx_t count0 = 0, count4 = 0;

        /* count digits 0 and 4 in base‑12 representation */
        while (temp != (seq_t)0) {
            idx_t digit = (idx_t)(temp % (seq_t)12);
            if (digit == 0)
                ++count0;
            else if (digit == 4)
                ++count4;
            temp /= (seq_t)12;
        }

        if (count0 > 0 && count0 == count4) {
            /* store term */
            ensure_cache_capacity(term_count + 1);
            cache[term_count++] = candidate;
        }

        last_checked = candidate;
    }

    return cache[n - 1];
}
