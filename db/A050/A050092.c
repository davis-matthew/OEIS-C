/*
 * A050092  a(n) = floor(a(n-1)/3) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 7*a(n-1).
 * https://oeis.org/A050092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1.
   For n > 1,
        x = floor(a(n-1) / 3)
        if x > 0 and x has not already occurred in the sequence,
               a(n) = x
        else
               a(n) = 7 * a(n-1)                     */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* cache[0] = a(1)                 */
static size_t cache_size = 0;    /* number of terms currently stored */
static size_t cache_cap  = 0;    /* allocated capacity of the array  */

/* test whether v has already appeared in the cache                     */
static bool seen_value(seq_t v) {
    for (size_t i = 0; i < cache_size; ++i)
        if (cache[i] == v)
            return true;
    return false;
}

/* grow the cache to at least 'needed' elements                         */
static void grow_cache(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ensure that terms 1 … n are stored in the cache                      */
static void ensure_terms(idx_t n) {
    /* n is 1‑based; cache_size holds the number of terms stored      */
    if ((size_t)n <= cache_size)
        return;
    grow_cache((size_t)n);
    while (cache_size < (size_t)n) {
        seq_t next;
        if (cache_size == 0) {            /* a(1) */
            next = (seq_t)1;
        } else {
            seq_t prev = cache[cache_size - 1];
            seq_t div  = prev / 3;
            if (div > (seq_t)0 && !seen_value(div))
                next = div;
            else
                next = prev * (seq_t)7;
        }
        cache[cache_size++] = next;
    }
}

seq_t a(idx_t n) {
    /* n is assumed to be >= 1                                     */
    ensure_terms(n);
    return cache[(size_t)n - 1];
}
