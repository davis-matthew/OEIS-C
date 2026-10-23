/*
 * A186264  Expansion of 3F2( 1, 3/2, 3/2; 3, 4;16 x).
 * https://oeis.org/A186264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186264:  a(n) = 3/(n+3) * Catalan(n+1)^2
 * Recurrence: a(0) = 1,
 *              a(n) = 4*(2n+1)^2 / ((n+3)*(n+2)) * a(n-1)   for n ≥ 1.
 *
 * Implementation uses a memoised array.  Intermediate products are
 * reduced by gcds to keep numbers as small as possible.  When a
 * 128‑bit unsigned type is available and seq_t fits in 64 bits,
 *   (unsigned __int128) is used for the multiplication to avoid
 *   overflow for the 64‑bit builds.  Otherwise the calculation is
 *   performed entirely with seq_t arithmetic, which is safe for the
 *   arbitrary‑precision build.
 *
 * Required helper: "../utils/gcd.h" provides gcd(seq_t,seq_t).
 * The build supplies seq_t and idx_t.  No other external headers
 * other than <stdlib.h> and <stdint.h> are used.
 *
 * The function a(idx_t) may be called in any order; the cache grows
 * lazily to accommodate the requested index.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

/* cache for previously computed values */
static seq_t *cache   = NULL;
static size_t cacheSize = 0;   /* number of computed entries */
static size_t cacheAlloc = 0;  /* allocated capacity */

/* ensure that values up to given index are cached */
static void
ensure_cache(idx_t n)
{
    if ((idx_t)n < cacheSize)                 /* all needed already computed */
        return;

    /* allocate / grow cache */
    size_t required = (size_t)n + 1;
    if (required > cacheAlloc) {
        size_t newAlloc = cacheAlloc == 0 ? 8 : cacheAlloc * 2;
        if (newAlloc < required) newAlloc = required;
        seq_t *tmp = (seq_t *)realloc(cache, newAlloc * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* out of memory */
        cache = tmp;
        cacheAlloc = newAlloc;
    }

    /* initialise a(0) if this is the first time */
    if (cacheSize == 0) {
        cache[0] = (seq_t)1;
        cacheSize = 1;
    }

    /* compute subsequent terms */
    for (size_t i = cacheSize; i <= (size_t)n; ++i) {
        /* --- compute numerator and denominator of the coefficient --- */
        /* t   = 2*i + 1                              */
        /* num = 4 * t^2                               */
        /* den = (i+3) * (i+2)                         */
        seq_t t   = ((seq_t)i * 2U) + 1U;
        seq_t num = (seq_t)4U * t * t;
        seq_t den = ((seq_t)i + 3U) * ((seq_t)i + 2U);

        /* reduce num/den by gcd */
        seq_t g = gcd(num, den);
        if (g != 1U) {
            num /= g;
            den /= g;
        }

        /* reduce denominator with previous value to keep product small */
        seq_t prev = cache[i-1];
        seq_t g2   = gcd(prev, den);
        if (g2 != 1U) {
            prev /= g2;
            den /= g2;
        }

        /* at this point num and den are coprime;
         * but num may still share factors with den, reduce them */
        seq_t g3 = gcd(num, den);
        if (g3 != 1U) {
            num /= g3;
            den /= g3;
        }

#ifdef __SIZEOF_INT128__
        /* if seq_t is a 64‑bit integer, use an unsigned __int128
           intermediate to avoid overflow for the 64‑bit builds. */
        if (sizeof(seq_t) <= 8) {
            __uint128_t prod = (__uint128_t)prev * ( __uint128_t)num;
            prod /= (__uint128_t)den;
            cache[i] = (seq_t)prod;
        } else
#endif
        {
            /* arbitrary‑precision build or no 128‑bit support:
               perform the multiplication/division with seq_t itself. */
            seq_t prod = prev * num;   /* multiplication may be large for big‑int */
            cache[i] = prod / den;
        }
    }

    cacheSize = (size_t)n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
