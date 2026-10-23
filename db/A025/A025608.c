/*
 * A025608  Number of n-move rook paths on 8 X 8 board from given corner to
 *          opposite corner.
 * https://oeis.org/A025608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 18*a(n-1) - 44*a(n-2) - 168*a(n-3); a(0) = 0, a(1) = 0,
 *            a(2) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values */
static seq_t *cache      = NULL; /* array of computed terms */
static size_t cache_cap  = 0;    /* allocated capacity */
static size_t cache_len  = 0;    /* number of terms computed (>= 3 after init) */

seq_t a(idx_t n)
{
    /* Handle the first few terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Initialise cache on the first call */
    if (cache_cap == 0) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initial values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
        cache_len = 3;
    }

    /* Grow cache if necessary   */
    if ((size_t)n >= cache_cap) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing entries using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
#if defined(__SIZEOF_INT128__) && (__SIZEOF_INT128__ == 16)
        /* When seq_t is an integer type that fits in 64 bits,
           use a 128‑bit intermediate to avoid overflow.      */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 temp = (unsigned __int128)cache[i-1] * 18;
            temp -= (unsigned __int128)cache[i-2] * 44;
            temp -= (unsigned __int128)cache[i-3] * 168;
            val = (seq_t)temp;
        } else
#endif
        {
            val = cache[i-1] * 18 - cache[i-2] * 44 - cache[i-3] * 168;
        }
        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
