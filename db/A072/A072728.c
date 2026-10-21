/*
 * A072728  Numerator of rationals >= 1 whose continued fractions consist only
 *          of 1's and 2's, in ascending order by the sum of the continued
 *          fraction terms and descending by lowest order continued fraction
 *          terms to highest.
 * https://oeis.org/A072728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation enumerates all simple continued fractions
   whose partial quotients consist only of 1 and 2, in the order
   specified by OEIS A072728: first by the sum of the terms,
   then by descending lexicographic order of the terms.
   The generated numerators are cached for O(1) subsequent
   look‑ups.  The algorithm is sufficient for n up to several
   thousand in the required time limit. */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of values already computed */
static seq_t *cache      = NULL;   /* array of numerators              */
static size_t cache_cap  = 0;       /* allocated capacity               */
static size_t cache_size = 0;       /* number of computed entries       */
static unsigned next_sum  = 0;      /* next total sum to generate       */
static unsigned cf[256];           /* buffer for a composition (max depth) */

/* recursive generator for a given remaining sum and depth */
static void rec_generate(unsigned rem, unsigned depth)
{
    if (cache_size >= cache_cap)
        return;

    if (rem == 0) {
        /* canonical check: last term may be 1 only if length==1 */
        if (depth > 1 && cf[depth-1] == 1)
            return;

        /* evaluate the continued fraction to obtain the numerator */
        seq_t p = cf[depth-1];
        seq_t q = (seq_t)1;
        for (int k = (int)depth-2; k >= 0; --k) {
            seq_t t = p;
            p = (seq_t)cf[k] * p + q;
            q = t;
        }
        cache[cache_size++] = p;
        return;
    }

    /* try term 2 first (descending order) */
    if (rem >= 2) {
        cf[depth] = 2;
        rec_generate(rem-2, depth+1);
        if (cache_size >= cache_cap)
            return;
    }

    /* then try term 1 */
    if (rem >= 1) {
        cf[depth] = 1;
        rec_generate(rem-1, depth+1);
    }
}

/* enlarge the cache if necessary and generate values until index n is available */
static void ensure_cache(idx_t n)
{
    if ((idx_t)(n + 1) <= (idx_t)cache_size)
        return;

    /* enlarge the cache if needed */
    if ((idx_t)(n + 1) > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 20000;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – abort */
            exit(1);
        }
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* generate sums incrementally until we have enough entries */
    while (cache_size <= (size_t)n) {
        ++next_sum;
        rec_generate(next_sum, 0);
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
