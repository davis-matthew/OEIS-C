/*
 * A044135  Numbers k such that string 5,3 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_53
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044135:
   a(n) = the n‑th integer k such that the base‑6 representation of k
   contains the substring 53, while the base‑6 representation of k-1 does
   not.
   The routine builds the sequence lazily: it keeps an internal
   cache of all previous terms and continues searching only until
   the requested index is filled. */

#include <stddef.h>   /* for idx_t */
#include <stdbool.h>  /* for bool */
#include <stdlib.h>   /* for malloc, free, realloc */
#include <string.h>   /* for memcpy */
#include "../utils/has_53.h"

/*---  dynamic cache of already computed terms  */
static seq_t *cache = NULL;   /* array holding terms  */
static idx_t cache_count = 0; /* number of cached terms */
static size_t cache_cap = 0;   /* allocated capacity of cache */
static seq_t last_k = 0;      /* last examined integer */

/*---  ensure the cache is large enough for 'needed' terms  */
static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < (size_t)needed)
        new_cap = (size_t)needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* should not happen in the test harness */
    if (cache) {
        memcpy(new_cache, cache, cache_count * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/*---  the required OEIS function  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but harmless fallback */

    /* Generate terms until we have a(n). */
    while (cache_count < n) {
        last_k++;  /* next integer to test */
        if (has_53(last_k) && !has_53(last_k - 1)) {
            ensure_capacity(cache_count + 1);
            cache[cache_count] = last_k;
            cache_count++;
        }
    }
    return cache[n - 1];
}
