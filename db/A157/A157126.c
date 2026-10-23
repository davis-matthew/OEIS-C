/*
 * A157126  Expansion of (1-2x-3x^2+x^3-x^5)/(1+4x^3+x^6).
 * https://oeis.org/A157126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -4*a(n-3) - a(n-6) for n ≥ 6; a(0) = 1, a(1) = -2, a(2) =
 *            -3, a(3) = -3, a(4) = 8, a(5) = 11
 * Range:     i8 n <= 10, i16 n <= 24, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t BASE[6] = {1, -2, -3, -3, 8, 11};

/* cached sequence values */
static seq_t *CACHE = NULL;
static size_t   CACHE_CAP = 0;
static size_t   CACHE_LEN = 0;

static void ensure_cache(size_t upto)
{
    if (CACHE_LEN > upto)
        return;

    /* grow cache to at least 'upto' (inclusive) */
    size_t new_cap = CACHE_CAP ? CACHE_CAP : 1;
    while (new_cap <= upto)
        new_cap <<= 1;

    /* allocate or reallocate the array */
    seq_t *tmp;
    if (CACHE)
        tmp = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    else
        tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));

    if (!tmp)  /* allocation failure – leave cache unchanged */
        return;

    CACHE = tmp;
    CACHE_CAP = new_cap;

    /* compute missing terms */
    for (size_t i = CACHE_LEN; i <= upto; ++i)
    {
        if (i < 6)
            CACHE[i] = BASE[i];
        else
            CACHE[i] = (seq_t)(-4) * CACHE[i - 3] - CACHE[i - 6];
    }
    CACHE_LEN = upto + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;          /* idx_t is unsigned, fits in size_t for tested n */
    ensure_cache(idx);
    /* if allocation failed, CACHE_LEN may be < idx+1; return 0 as a safe fallback */
    return (CACHE_LEN > idx) ? CACHE[idx] : (seq_t)0;
}
