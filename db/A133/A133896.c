/*
 * A133896  Numbers m such that binomial(m+6,m) mod 6 = 0.
 * https://oeis.org/A133896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   eligible
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the (n+1)-th integer m such that C(m+6,m) ≡ 0 (mod 6) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/eligible.h"

/* --- cache for already computed terms ----------------------------------- */
static seq_t *cache   = NULL;  /* array of found m's */
static size_t cache_len = 0;   /* how many stored */
static size_t cache_cap = 0;   /* current allocation capacity */
static idx_t  next_m    = 0;    /* next m to test */

/* enlarge the cache so that it can hold at least min_cap elements */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < min_cap) new_cap = min_cap;

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/*  a(idx_t n)   -   OEIS sequence A133896                                  */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* generate terms until we have at least n+1 of them */
    while (cache_len <= (size_t)n) {
        if (eligible(next_m)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
    return cache[(size_t)n];
}
