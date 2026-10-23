/*
 * A018230  Numbers k such that normalizer of Gamma_0(k) is triangle group
 *          (2,6,inf).
 * https://oeis.org/A018230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   The sequence consists of the numbers * 3·4^i (i = 0,1,2,3) * and
 *            * 27·4^j (j = 0,1,2,…) * sorted in ascending order, so the first
 *            terms are * 3,12,27,48,108,192,432,1728,…
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to keep already generated values */
    static seq_t *cache = NULL;     /* array of generated terms */
    static size_t cache_len = 0;    /* number of terms stored */
    static size_t cache_cap = 0;    /* current capacity of the array */

    /* generation state for the two geometric progressions */
    static idx_t idx1 = 0;          /* index in the first progression (i) */
    static idx_t idx2 = 0;          /* index in the second progression (j) */
    static seq_t val1 = 3;          /* current value of 3·4^i  (valid only while idx1≤3) */
    static seq_t val2 = 27;         /* current value of 27·4^j */

    /* initialise everything on the very first call */
    if (cache_len == 0) {
        cache_cap = 16;                    /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory – return zero */
    }

    /* Grow the cache if we do not yet have enough terms */
    if (n > cache_len) {
        /* Ensure enough capacity */
        size_t needed = n;
        if (cache_cap < needed) {
            size_t new_cap = cache_cap;
            while (new_cap < needed) new_cap <<= 1;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) return (seq_t)0;  /* out of memory */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Generate terms until we have at least n of them */
        while (cache_len < n) {
            bool use_val1;
            if (idx1 <= 3)
                use_val1 = val1 <= val2;  /* compare the two candidates */
            else
                use_val1 = false;         /* first progression exhausted */

            seq_t next;
            if (use_val1) {
                next = val1;
                ++idx1;
                if (idx1 <= 3)            /* advance the first progression */
                    val1 *= (seq_t)4;
            } else {
                next = val2;
                ++idx2;
                val2 *= (seq_t)4;          /* advance the second progression */
            }

            cache[cache_len++] = next;
        }
    }

    /* n is 1‑based */
    return cache[n - 1];
}
