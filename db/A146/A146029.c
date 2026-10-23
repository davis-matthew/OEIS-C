/*
 * A146029  Numbers that can be written from base 2 to base 17 using only the
 *          digits 0 to 8 (conjectured to be complete).
 * https://oeis.org/A146029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A146029: numbers that, in each base 10 … 17,
   have only the digits 0–8.  The terms are produced
   by a simple brute‑force scan.  The first
   few terms are 0,1,2,3,4,5,6,7,8,17,18,… */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Check whether x satisfies the digit condition for all bases 10..17. */
static bool good(seq_t x)
{
    for (int base = 10; base <= 17; ++base)
    {
        seq_t tmp = x;
        while (tmp != (seq_t)0)
        {
            seq_t d = tmp % (seq_t)base;
            if (d > (seq_t)8)
                return false;
            tmp /= (seq_t)base;
        }
    }
    return true;
}

/* Static storage for the sequence values. */
static seq_t  *_cache        = NULL;     /* array of found values   */
static size_t  _cache_size   = 0;        /* number of values stored */
static size_t  _cache_cap    = 0;        /* array capacity          */
static idx_t   _next_candidate = (idx_t)0;   /* next integer to test   */

/* Append a value to the cache, growing the array if necessary. */
static void cache_append(seq_t val)
{
    if (_cache_size == _cache_cap)
    {
        size_t new_cap = _cache_cap ? _cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                       /* allocation failure     */
        _cache      = tmp;
        _cache_cap  = new_cap;
    }
    _cache[_cache_size++] = val;
}

/* Maximum value representable in seq_t, when it fits in idx_t. */
static const idx_t seq_t_max_val =
    (sizeof(seq_t) <= sizeof(idx_t)) ? (idx_t)( ~(seq_t)0 ) : (idx_t)0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined offset – return 0 */

    /* Generate terms until we have at least n of them. */
    while (_cache_size < (size_t)n)
    {
        /* Stop if we would overflow seq_t. */
        if ((sizeof(seq_t) <= sizeof(idx_t)) &&
            (idx_t)_next_candidate > seq_t_max_val)
            break;

        seq_t cur = (seq_t)_next_candidate;
        if (good(cur))
            cache_append(cur);
        ++_next_candidate;
    }

    /* n is 1‑based; the cache is 0‑based. */
    return _cache[n - 1];
}
