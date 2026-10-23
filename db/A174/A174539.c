/*
 * A174539  Starting numbers n such that the number of halving and tripling
 *          steps to reach 1 under the Collatz 3x+1 map is a perfect square.
 * https://oeis.org/A174539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _collatz_steps
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174539 – the n‑th integer whose Collatz total stopping time is
   a perfect square.  The function builds an internal cache, so it works for
   calls in any order.  Collatz stepping is performed with 64‑bit values to
   avoid overflow of very small seq_t types. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_collatz_steps.h"

/*--- Integer square root for 64‑bit values --------------------------------*/
static inline idx_t _isqrt_u64(idx_t v)
{
    idx_t r = 0;
    for (int i = 62; i >= 0; --i) {
        idx_t cand = r | ((idx_t)1 << i);
        if ((unsigned __int128)cand * cand <= v)
            r = cand;
    }
    return r;
}

/*--- Check perfect square -----------------------------------------------*/
static bool _is_square(idx_t v)
{
    idx_t r = _isqrt_u64(v);
    return r * r == v;
}

/*--- Memoization cache -----------------------------------------------*/
static seq_t *cache = NULL;       /* A(1), A(2), … of type seq_t */
static size_t cache_len = 0;      /* number of elements stored */
static size_t cache_cap = 0;      /* capacity of cache array   */
static idx_t next_candidate = 1;  /* next integer to test */

/* grow the cache when needed */
static void _grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp)
            cache = tmp;
    }
}

/*--- Public sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not used by the OEIS framework */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t cand = next_candidate++;
        idx_t steps = _collatz_steps(cand);
        if (_is_square(steps)) {
            if (cache_len == cache_cap)
                _grow_cache();
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
