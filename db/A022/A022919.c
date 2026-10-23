/*
 * A022919  Multinomial coefficients(TOP, BOTTOM), where TOP = n^2, BOTTOM = (
 *          1 3 5 ... 2n-1 ).
 * https://oeis.org/A022919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022919: 
   a(n) = (n^2)! / ∏_{k=1..n} (2k-1)!          (a(0)=1, a(1)=1)
   Recurrence: a(n) = a(n-1) * binomial(n^2, (n-1)^2) for n ≥ 1
*/

#include <stdlib.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */

static seq_t *cache     = NULL;   /* memoised values */
static size_t cache_cap = 0;      /* allocated capacity       */
static idx_t   cache_last = 0;    /* highest index computed   */

/* enlarge cache so that indices 0 … n can be stored */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;      /* already large enough */

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache if needed */
    if (cache == NULL) {
        ensure_cache(0);
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_last = 0;
    }

    /* trivial cases */
    if (n == 0) return cache[0];
    if (n == 1) return (seq_t)1;       /* a(1) = 1 */

    /* make sure cache can hold all indices up to n */
    ensure_cache(n);

    /* already computed ? */
    if (n <= cache_last) return cache[n];

    /* compute missing values up to n, starting from cache_last+1 */
    seq_t prev = cache[cache_last];
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* compute binomial(i^2, (i-1)^2) */
        idx_t top  = (idx_t)i * i;                  /* i^2           */
        idx_t bot  = (idx_t)(i-1) * (i-1);          /* (i-1)^2       */
        seq_t factor = binomial(top, bot);
        prev = prev * factor;
        cache[i] = prev;
    }
    cache_last = n;
    return cache[n];
}
