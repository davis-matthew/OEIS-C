/*
 * A043214  Numbers k such that 6 and 8 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_68_86, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043214: numbers k such that the base‑9 representation of k contains
   adjacent digits 6 and 8 (either “68” or “86”) but the base‑9
   representation of k‑1 does not.  The routine searches forwards from
   the last candidate and memoises terms so that any n ≥ 1 can be
   requested in arbitrary order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"
#include "../utils/has_adjacent_68_86.h"

/* Memoisation cache */
static seq_t *cache      = NULL;   /* stored terms                 */
static size_t cache_len  = 0;      /* number of terms stored       */
static size_t cache_cap  = 0;      /* allocated capacity           */
static seq_t next_k      = 1;      /* next number to test          */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *p = seq_realloc(cache, cache_cap, new_cap);
    if (!p)
        abort();

    cache   = p;
    cache_cap = new_cap;
}

/* Required function. */
seq_t a(idx_t n)
{
    /* Build terms up to the requested index. */
    while (cache_len < (size_t)n) {
        if (has_adjacent_68_86(next_k) && !has_adjacent_68_86(next_k - 1)) {
            ensure_cache(cache_len + 1);
            cache[cache_len] = next_k;
            ++cache_len;
        }
        ++next_k;
    }
    return cache[n - 1];
}
