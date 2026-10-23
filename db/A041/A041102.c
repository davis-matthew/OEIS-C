/*
 * A041102  Numerators of continued fraction convergents to sqrt(59).
 * https://oeis.org/A041102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1060*a(n-6) - a(n-12)
   with initial values
     a(0) = 7, a(1) = 8, a(2) = 23, a(3) = 169, a(4) = 361, a(5) = 530,
     a(6) = 7781, a(7) = 8311, a(8) = 24403, a(9) = 179132,
     a(10) = 382667, a(11) = 561799
   for all n ≥ 0. */

#include <stdlib.h>

/* immutable initial terms */
static const seq_t init_vals[12] = {
    7,          /* a(0) */
    8,          /* a(1) */
    23,         /* a(2) */
    169,        /* a(3) */
    361,        /* a(4) */
    530,        /* a(5) */
    7781,       /* a(6) */
    8311,       /* a(7) */
    24403,      /* a(8) */
    179132,     /* a(9) */
    382667,     /* a(10) */
    561799      /* a(11) */
};

/* cached computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of entries actually stored */

seq_t a(idx_t n)
{
    /* ensure cache can hold index n */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)
            return 0;   /* out‑of‑memory: graceful fallback */

        cache = new_cache;
        cache_len = new_len;

        /* initialise initial terms if they are now inside the cache */
        size_t upto = (cache_len < 12) ? cache_len : 12;
        for (size_t i = 0; i < upto; ++i)
            cache[i] = init_vals[i];
    }

    /* compute missing terms up to n */
    if (n >= 12)
    {
        for (size_t i = 12; i <= (size_t)n; ++i)
        {
            /* recurrence: a(i) = 1060*a(i-6) - a(i-12) */
            cache[i] = 1060 * cache[i - 6] - cache[i - 12];
        }
    }

    return cache[n];
}
