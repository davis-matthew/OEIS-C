/*
 * A041745  Denominators of continued fraction convergents to sqrt(392).
 * https://oeis.org/A041745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041745: Denominators of continued fraction convergents to sqrt(392)
   Recurrence: a(n) = 198*a(n-4) - a(n-8)  for n > 7
   Initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=5, a(4)=194, a(5)=199,
                    a(6)=791, a(7)=990
   The implementation memoises intermediate terms in a dynamic array
   to support any order of calls.  All arithmetic is performed with
   the type seq_t supplied by the build environment.  The cache is
   initialised with a zero out‑of‑band value to detect uncomputed
   entries.  The code complies with C11 and C++17.  No external
   utilities are required. */

#include <stdlib.h>
#include <string.h>

/* External integer types provided by the build */
typedef uint64_t idx_t;
/* seq_t is defined in the build environment (uint8_t …, unsigned __int128,
   or an arbitrary‑precision integer class). */

/* Local copy of the first eight terms */
static const seq_t _base[8] = {
    (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)5,
    (seq_t)194, (seq_t)199, (seq_t)791, (seq_t)990
};

/* Memoisation cache */
static seq_t * _cache    = NULL;
static size_t _cache_len = 0;

/* Ensure the cache is large enough and initialise new slots to zero. */
static void _ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (_cache_len < needed) {
        size_t old_len = _cache_len;
        seq_t *tmp = (seq_t *)realloc(_cache, needed * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – fallback: rebuild cache from scratch. */
            free(_cache);
            _cache      = NULL;
            _cache_len  = 0;
            _ensure_cache(n);
            return;
        }
        /* New slots are undefined – set to zero. */
        memset(tmp + old_len, 0, (needed - old_len) * sizeof(seq_t));
        _cache     = tmp;
        _cache_len = needed;
    }

    /* Initialise the first eight entries with the known seed values. */
    for (size_t i = 0; i < 8; ++i)
        _cache[i] = _base[i];
}

/* Compute any missing terms up to index n. */
static void _fill_cache(idx_t n)
{
    for (size_t i = 8; i <= (size_t)n; ++i) {
        if (_cache[i] == (seq_t)0) {          /* not yet computed */
            /* Use the recurrence a(n) = 198*a(n-4) - a(n-8) */
            _cache[i] = (198 * _cache[i - 4]) - _cache[i - 8];
        }
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n < 8)                           /* simple lookup for seed */
        return _base[(size_t)n];

    _ensure_cache(n);
    _fill_cache(n);
    return _cache[(size_t)n];
}
