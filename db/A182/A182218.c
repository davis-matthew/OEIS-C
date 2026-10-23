/*
 * A182218  Numbers derived as with Lucky Numbers, but beginning with the
 *          removal of every third number instead of every second.
 * https://oeis.org/A182218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Lucky numbers obtained by first deleting every third element of
 *            1,2,3,… then deleting every k‑th remaining element where k runs
 *            through the surviving numbers greater than the previous step;
 *            i.e. 3,4,7,8,10,…
 * Range:     u8 n <= 60, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External types supplied by the build system */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Cached survivors of the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Construct a cache containing at least n survivors.
   The routine grows the upper bound until it contains enough terms. */
static void build_cache(idx_t n)
{
    if (n == 0) return;
    idx_t bound = n * 10;          /* start with a generous guess          */
    if (bound < 100) bound = 100;  /* guard against very small n           */

    while (1) {
        size_t len = (size_t)bound;
        seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!arr) abort();          /* allocation failure                    */

        /* initialise the array with 1,2,…,bound */
        for (idx_t i = 0; i < len; ++i) arr[i] = (seq_t)(i + 1);

        /* first elimination: delete every third element */
        size_t new_len = 0;
        for (size_t i = 0; i < len; ++i)
            if ((i + 1) % 3 != 0) arr[new_len++] = arr[i];
        len = new_len;

        /* subsequent eliminations starting with the third element
           (index 2, 0‑based) in the current list */
        size_t idx = 2;
        while (idx < len) {
            seq_t step = arr[idx];
            if ((size_t)step > len) break;   /* nothing left to delete      */
            new_len = 0;
            for (size_t j = 0; j < len; ++j)
                if ((j + 1) % step != 0) arr[new_len++] = arr[j];
            len = new_len;
            idx++;
        }

        /* copy the survivors to the permanent cache */
        seq_t *new_cache = (seq_t *)malloc(len * sizeof(seq_t));
        if (!new_cache) abort();
        memcpy(new_cache, arr, len * sizeof(seq_t));
        free(arr);

        /* replace the old cache, if any */
        if (cache) free(cache);
        cache = new_cache;
        cache_len = len;

        /* success or retry with a larger bound */
        if (cache_len >= (size_t)n) return;
        bound = bound * 2;
    }
}

/* Public function required by the platform */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((idx_t)cache_len < n) build_cache(n);
    return cache[(size_t)(n - 1)];
}
