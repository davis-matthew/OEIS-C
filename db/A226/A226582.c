/*
 * A226582  Smallest number of integer-sided squares needed to tile a 9 X n
 *          rectangle.
 * https://oeis.org/A226582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226582: Smallest number of integer‑sided squares needed to tile a 9×n rectangle.
   Formula: a(n) = 1 + a(n-9) for n > 10, with
   a(0)=0, a(1)=9, a(2)=6, a(3)=3, a(4)=6, a(5)=6, a(6)=3, a(7)=6,
   a(8)=7, a(9)=1, a(10)=6. */

#include <stdlib.h>
#include <stddef.h>

/* Initial values for n = 0 .. 10 */
static const seq_t _init[11] = {
    0, 9, 6, 3, 6, 6, 3, 6, 7, 1, 6
};

/*  Cache for already‑computed values  */
static seq_t * _cache = NULL;   /* pointer to array of seq_t */
static size_t  _cache_len = 0;  /* number of valid elements in _cache */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure cache is initialized up to at least 11 elements. */
    if (_cache_len == 0) {
        _cache_len = 11;
        _cache = (seq_t*)malloc(_cache_len * sizeof(seq_t));
        if (!_cache) abort();          /* out of memory */
        for (idx_t i = 0; i < 11; ++i) {
            _cache[i] = _init[i];
        }
    }

    /* Expand cache if required. */
    if ((idx_t)_cache_len <= n) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(_cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();        /* out of memory */
        _cache = new_cache;
        /* Compute new entries */
        for (idx_t i = (idx_t)_cache_len; i <= n; ++i) {
            if (i < 11) {
                _cache[i] = _init[i];
            } else {
                /* a(i) = 1 + a(i-9) for i > 10 */
                _cache[i] = (seq_t)1 + _cache[i - 9];
            }
        }
        _cache_len = new_len;
    }

    return _cache[(idx_t)n];
}
