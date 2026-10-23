/*
 * A044389  Numbers n such that string 5,7 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044389: numbers n such that the decimal string "57" occurs
   in n but not in n-1.  The function generates terms on demand and
   caches them, so any call order is efficient. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static bool has_57(uint64_t x)
/* Returns true iff the decimal representation of x contains the
   substring "57". */
{
    if (x < 57) return false;
    uint64_t prev = x % 10;
    x /= 10;
    while (x) {
        uint64_t cur = x % 10;
        if (cur == 5 && prev == 7) return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
/* Return the n-th term of sequence A044389 (1‑based index). */
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, but harmless */

    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* candidate counter – the next integer that will be tested */
    static uint64_t candidate = 57;

    /* Grow cache until we have at least n terms */
    while (cache_len < n) {
        /* Find the next valid number */
        while (!(has_57(candidate) && !has_57(candidate - 1))) {
            ++candidate;
        }

        /* Ensure room in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
            seq_t *new_buf = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!new_buf) { abort(); }          /* allocation failed */
            if (cache) {
                memcpy(new_buf, cache, cache_len * sizeof(seq_t));
                free(cache);
            }
            cache = new_buf;
            cache_cap = new_cap;
        }

        /* Store the term and advance */
        cache[cache_len++] = (seq_t)candidate;
        ++candidate;
    }

    return cache[n - 1];
}
