/*
 * A044310  Numbers k such that the string 6,5 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has65
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) computed by an incremental search for all integers k such that
   the base‑9 representation of k contains the substring "65" but
   the base‑9 representation of k-1 does not.  The sequence is indexed
   starting at 1 (a(1) = 59). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has65.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static seq_t  last_candidate = 0;  /* last number examined */

static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

static void generate_until(idx_t n)
{
    while (cache_size < (size_t)n) {
        last_candidate++;

        if (has65(last_candidate) && !has65(last_candidate - 1)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = last_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Offset is 1; undefined for n = 0 */
    generate_until(n);
    return cache[n - 1];
}
