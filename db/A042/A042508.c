/*
 * A042508  Numerators of continued fraction convergents to sqrt(782).
 * https://oeis.org/A042508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1566*a(n-4) – a(n-8)
   with initial values
   a(0)=27, a(1)=28, a(2)=755, a(3)=783,
   a(4)=43037, a(5)=43820, a(6)=1182357, a(7)=1226177 */

#include <stdlib.h>
#include <string.h>   /* for memcpy, if desired */

static seq_t *seq_cache = NULL;      /* cached terms              */
static size_t cache_cap   = 0;       /* current capacity          */
static idx_t  max_computed = (idx_t)-1; /* highest index computed  */

static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;

    /* ensure at least space for the eight initial terms */
    if (needed < 8) needed = 8;

    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;

    /* cast needed for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();       /* allocation failed */
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* first eight terms are known a priori */
    static const seq_t init[8] = {
        27, 28, 755, 783,
        43037, 43820, 1182357, 1226177
    };

    if (n < 8) return init[n];   /* exact values for offset */

    ensure_cache((size_t)n + 1);

    /* initialize cache on first request for n >= 8 */
    if (max_computed == (idx_t)-1) {
        for (idx_t i = 0; i < 8; ++i) seq_cache[i] = init[i];
        max_computed = 7;
    }

    /* if already computed, just return */
    if (n <= max_computed) return seq_cache[(size_t)n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_cache[(size_t)i] =
            (seq_t)1566 * seq_cache[(size_t)(i - 4)] -
            seq_cache[(size_t)(i - 8)];
    }
    max_computed = n;
    return seq_cache[(size_t)n];
}
