/*
 * A235304  Number of (n+1) X (2+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = *
 *            4*a(n-1)+25*a(n-2)-112*a(n-3)-231*a(n-4)+1260*a(n-5)+875*a(n-6)
 *            *
 *            -7280*a(n-7)-224*a(n-8)+22736*a(n-9)-7980*a(n-10)-36288*a(n-11)
 *            * +21456*a(n-12)+23040*a(n-13)-17280*a(n-14) * * with initial
 *            values: * a(1)=210, a(2)=518, a(3)=1106, a(4)=2918, a(5)=6786, *
 *            a(6)=18662, a(7)=46226, a(8)=13...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifndef SEQ_T_DEFINED
/* The build will provide a definition for seq_t and idx_t; do not redefine here. */
#endif

/* Initial 14 terms (index 0 unused). */
static const seq_t _init[15] = {
    0ULL,        /* unused */
    210ULL,      /* a(1) */
    518ULL,      /* a(2) */
    1106ULL,     /* a(3) */
    2918ULL,     /* a(4) */
    6786ULL,     /* a(5) */
    18662ULL,    /* a(6) */
    46226ULL,    /* a(7) */
    130598ULL,   /* a(8) */
    338370ULL,   /* a(9) */
    973478ULL,   /* a(10) */
    2602706ULL,  /* a(11) */
    7582118ULL,  /* a(12) */
    20721666ULL, /* a(13) */
    60895142ULL  /* a(14) */
};

/* Cached values.  We allocate lazily on first use. */
static seq_t * _cache = NULL;
static idx_t _cache_size = 0;   /* largest computed index (>=14 after init) */

/* Ensure the cache is large enough to hold up to idx `n`.  If it is
 * uninitialized, allocate it and fill the first 14 terms. */
static void _ensure_cache(idx_t n)
{
    if (_cache == NULL) {
        /* Allocate space for at least the first 15 indices (0..14). */
        _cache = (seq_t*)malloc(sizeof(seq_t) * 15);
        if (_cache == NULL) {
            /* Out of memory: signal error by returning 0 (undefined). */
            return;
        }
        for (idx_t i = 1; i <= 14; ++i)
            _cache[i] = _init[i];
        _cache_size = 14;
        /* Zero unused indexes to avoid accidental use. */
        _cache[0] = 0;
    }

    if (n <= _cache_size)
        return;   /* already computed */

    /* Grow the cache if needed.  Expand to double the previous size,
     * or to n, whichever is larger. */
    size_t new_cap = _cache_size + 1;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(_cache, sizeof(seq_t) * new_cap);
    if (new_cache == NULL) {
        /* Realloc failed: retain old pointer and lose ability to grow. */
        return;
    }
    _cache = new_cache;

    /* Compute new terms one by one. */
    for (idx_t k = _cache_size + 1; k <= n; ++k) {
        seq_t val =
            (seq_t)4   * _cache[k-1] +
            (seq_t)25  * _cache[k-2] -
            (seq_t)112 * _cache[k-3] -
            (seq_t)231 * _cache[k-4] +
            (seq_t)1260 * _cache[k-5] +
            (seq_t)875  * _cache[k-6] -
            (seq_t)7280 * _cache[k-7] -
            (seq_t)224  * _cache[k-8] +
            (seq_t)22736 * _cache[k-9] -
            (seq_t)7980 * _cache[k-10] -
            (seq_t)36288 * _cache[k-11] +
            (seq_t)21456 * _cache[k-12] +
            (seq_t)23040 * _cache[k-13] -
            (seq_t)17280 * _cache[k-14];
        _cache[k] = val;
    }
    _cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    _ensure_cache(n);
    /* If allocation failed, _cache may be NULL; guard against that. */
    if (_cache == NULL) return 0;
    return _cache[n];
}
