/*
 * A160444  Expansion of g.f.: x^2*(1 + x - x^2)/(1 - 2*x^2 - 2*x^4).
 * https://oeis.org/A160444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 47, u64 n <= 91, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A160444
   Recurrence:
     a(1)=0, a(2)=1, a(3)=1, a(4)=1
     a(n)=2*a(n-2)+2*a(n-4)   for n≥5
   (All arithmetic is on the integer type seq_t.)
*/
#include <stdlib.h> /* for realloc */

static seq_t *cache = NULL;   /* cache[1..n] holds computed terms */
static size_t cache_size = 0; /* allocated array size */
static idx_t cache_max  = 0;  /* largest n with a(n) stored */

/* Return a(n) for the OEIS sequence A160444 */
seq_t a(idx_t n)
{
    /* --- initialize the cache on first use --------------------------------- */
    if (cache == NULL) {
        /* allocate space for indices 0..4 (index 0 unused) */
        size_t init_size = 5;
        cache = (seq_t *)realloc(NULL, init_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */

        /* base values (indexing starts at 1) */
        cache[0] = (seq_t)0; /* unused */
        cache[1] = (seq_t)0; /* a(1) */
        cache[2] = (seq_t)1; /* a(2) */
        cache[3] = (seq_t)1; /* a(3) */
        cache[4] = (seq_t)1; /* a(4) */

        cache_size = init_size;
        cache_max  = 4;
    }

    /* --- handle small n directly -------------------------------------------- */
    if (n <= cache_max) return cache[n];

    /* --- grow cache if necessary ------------------------------------------- */
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    if (needed > cache_size) {
        size_t new_size = cache_size * 2;
        if (new_size < needed) new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;         /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* --- compute new terms using the recurrence ----------------------------- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* recurrence only valid for i≥5; earlier values already set */
        cache[i] = 2 * cache[i - 2] + 2 * cache[i - 4];
    }
    cache_max = n;

    return cache[n];
}
