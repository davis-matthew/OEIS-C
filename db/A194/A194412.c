/*
 * A194412  Numbers m such that Sum_{k=1..m} (<1/3 + k*r> - <k*r>) = 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A194412 by incremental search.
   The sequence consists of the positive integers m such that the
   sum_{k=1..m} ( {1/3 + k*sqrt(2)} - {k*sqrt(2)} ) equals 0.
   For each m the condition reduces to counting how many of the
   first m fractional parts of k*sqrt(2) exceed 2/3.  If that
   count equals m/3 (and therefore m is a multiple of 3) then m
   is a sequence term.  The function builds the sequence on
   demand, remembering previous terms in a static cache.  It
   uses long double arithmetic for the fractional part,
   which is sufficient for the required range.  */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Static cache for already computed terms.  */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Current state of the incremental search.  */
static uint64_t cur_m       = 0;   /* last m examined */
static uint64_t breakpoint = 0;   /* number of k with frac(k*sqrt2)>2/3 */

/* Smallest multiple of 3 needed to grow the cache.  */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe */

    /* ensure cache contains at least n terms */
    while (cache_len < (size_t)n) {
        /* examine next m */
        cur_m++;

        /* fractional part of m*sqrt(2) using long double */
        long double prod  = (long double)cur_m * sqrtl(2.0L);
        long double frac  = prod - floorl(prod);

        if (frac > (2.0L / 3.0L))
            breakpoint++;          /* a_k > 2/3 */

        /* condition for a term: m multiple of 3 and B == m/3 */
        if ((cur_m % 3) == 0 && breakpoint == cur_m / 3) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cur_m;
        }
    }

    return cache[n - 1];
}
