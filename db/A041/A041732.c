/*
 * A041732  Numerators of continued fraction convergents to sqrt(386).
 * https://oeis.org/A041732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(386)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* static data shared between calls */
    static bool          initialized = false;
    static seq_t         a0;                 /* floor(sqrt(386)) */
    static uint64_t      a0_ull;              /* integer version of a0 for period calculation */
    static seq_t        *period = NULL;      /* array of continued‑fraction digits a1,a2,…,aL */
    static size_t        period_len = 0;
    static seq_t        *p_cache = NULL;     /* cache of numerators p[n] */
    static size_t        cache_size = 0;      /* current size of p_cache */

    /* --- 1. one‑time initialisation ---------------------------------------- */
    if (!initialized)
    {
        /* compute a0 = floor(sqrt(386)) */
        a0 = 0;
        for (uint64_t x = 0; x * x <= 386; ++x) a0 = x;   /* 19 */
        a0_ull = (uint64_t)a0;

        /* compute the periodic part of the continued fraction of sqrt(386)   */
        uint64_t m = 0, d = 1, a = a0_ull;
        size_t   cap = 32;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        period_len = 0;
        while (1)
        {
            m = d * a - m;
            d = (386ULL - m * m) / d;
            a = (a0_ull + m) / d;
            if (period_len >= cap)          /* grow the array if needed */
            {
                cap <<= 1;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[period_len++] = (seq_t)a;
            if (a == 2 * a0_ull) break;     /* end of period */
        }

        /* initialise the numerator cache with the first term */
        p_cache = (seq_t *)malloc(sizeof(seq_t));
        p_cache[0] = a0;                   /* p_0 = a0 */
        cache_size = 1;
        initialized = true;
    }

    /* --- 2. extend the cache up to index n -------------------------------- */
    if ((size_t)n >= cache_size)
    {
        size_t  new_size  = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(p_cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed – return 0 */

        /* compute all missing entries */
        for (size_t i = cache_size; i <= n; ++i)
        {
            seq_t ai;                      /* continued‑fraction digit a_i */
            if (i == 0)
                ai = a0;
            else
            {
                if (i == 1)
                    ai = period[0];
                else
                    ai = period[(i - 1) % period_len];
            }

            seq_t pi;                      /* numerator p_i */
            if (i == 0)
                pi = a0;
            else if (i == 1)
                pi = ai * new_cache[0] + (seq_t)1; /* use p_{-1} = 1 */
            else
                pi = ai * new_cache[i - 1] + new_cache[i - 2];

            new_cache[i] = pi;
        }

        p_cache    = new_cache;
        cache_size = new_size;
    }

    /* --- 3. return the cached value ---------------------------------------- */
    return p_cache[(size_t)n];
}
