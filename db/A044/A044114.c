/*
 * A044114  Numbers n such that string 2,0 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has20_base6_idx, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044114 – Numbers n such that the base‑6 representation of n contains
   the substring “20” but the base‑6 representation of n‑1 does not.
   a(1)=12, a(2)=48, a(3)=72, …
   The function a(idx_t n) returns the n‑th term of this sequence
   (with a(1) being the first listed term).  It searches forward
   from the last computed value and memoises the results.  The code
   uses only the operations permitted for seq_t and idx_t
   (no floating point, no <math.h>, no non‑standard APIs). */

/* Compile‑time includes – no build‑time includes needed. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has20_base6_idx.h"
#include "../utils/seq_free.h"

/* =========================================================================
   Simple dynamic array for seq_t values.
   ========================================================================= */
static seq_t *seq_alloc(size_t cap)
{
    return (seq_t *)malloc(cap * sizeof(seq_t));
}

/* Cache of previously found sequence values.  All pointers and sizes
   are static so that any order of calls to a(idx_t) will work. */
static seq_t *cache     = NULL; /* array of cached a(i) values      */
static size_t cache_len = 0;    /* number of values cached so far   */
static size_t cache_cap = 0;    /* current allocated capacity       */

/* =========================================================================
   Main sequence generator.
   ========================================================================= */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;        /* invalid index – return 0 as a sentinel */

    /* Return cached value if already computed. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* Starting point for the search.  If we have no cached values,
       begin at 1; otherwise begin just after the last cached value. */
    idx_t next_num = (cache_len == 0)
                     ? (idx_t)1
                     : (idx_t)((seq_t)cache[cache_len - 1] + (seq_t)1);

    /* Search until we have found the nth term. */
    while (cache_len < (size_t)n) {
        /* Test whether current number meets the definition. */
        bool cur_has20  = has20_base6_idx(next_num);
        bool prev_has20 = (next_num > 0) && has20_base6_idx(next_num - (idx_t)1);

        if (cur_has20 && !prev_has20) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = seq_alloc(new_cap);
                if (!new_cache) {           /* allocation failure */
                    seq_free(cache);
                    cache = NULL;
                    cache_len = cache_cap = 0;
                    return 0;
                }
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                seq_free(cache);
                cache      = new_cache;
                cache_cap  = new_cap;
            }

            /* Store the found term. */
            cache[cache_len++] = (seq_t)next_num;
        }
        ++next_num;
    }

    return cache[n - 1];
}
