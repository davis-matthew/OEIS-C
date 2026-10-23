/*
 * A044472  Numbers n such that string 2,4 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_24_base5, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A044472 – numbers n whose base‑5 representation contains
 * the substring “24” but the base‑5 representation of n+1 does not.
 *
 * This implementation performs a linear scan, checks each integer,
 * and memoises the results in a dynamic array so that repeated calls
 * are efficient.  It works for any seq_t size defined by the build.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/has_24_base5.h"

/* Memoised generator of a(n).  a(1) is the first term (14). */
seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;   /* dynamic array of found terms     */
    static size_t cache_len      = 0;      /* number of stored terms           */
    static size_t cache_cap      = 0;      /* current capacity of cache        */
    static idx_t last_checked    = 0;      /* next integer to examine (exclusive) */

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t candidate = (seq_t)last_checked + 1;

        if (has_24_base5(candidate) && !has_24_base5(candidate + 1)) {
            /* Resize cache when full. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) {            /* allocation failed – abort */
                    cache = NULL;
                    cache_len = 0;
                    cache_cap = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        last_checked++;
    }

    return cache[n - 1];
}
