/*
 * A097072  Expansion of (1 - 2*x + 2*x^2)/((1 - x^2)*(1 - 2*x)).
 * https://oeis.org/A097072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4·2ⁿ – 3 + 5·(–1)ⁿ)/6 * Equivalent recurrence: a(0)=1;
 *            a(1)=0; a(n)=a(n–1)+2·a(n–2)+1 for n≥2. * The recurrence uses
 *            only addition and a multiply‑by‑2 via addition, * suitable for
 *            any seq_t (fixed‑width or arbitrary precision)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array of already computed values   */
static size_t cache_size = 0;    /* number of values stored (>=2 if set) */
static size_t cache_cap  = 0;    /* allocated capacity of cache        */

/* helper: ensure cache has capacity for at least up_to indices (0‑based) */
static void ensure_cache(size_t up_to)
{
    if (up_to < cache_cap)
        return;

    size_t new_cap = (cache_cap > 0) ? cache_cap : 2;
    while (new_cap <= up_to)
        new_cap <<= 1;                 /* double until sufficient */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* should never happen in tests */

    /* copy old data and initialize rest */
    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* helper: extend the cache up to index n (inclusive) using recurrence */
static void extend_cache(idx_t n)
{
    /* n is idx_t (unsigned 64‑bit). For building the array we cast to size_t 
     * which is safe for the sizes used in testing (n up to few thousand).   */
    size_t up_to = (size_t)n;

    if (up_to < cache_size)
        return;          /* already computed */

    /* ensure array capacity first */
    ensure_cache(up_to);

    /* compute missing values using the recurrence */
    if (cache_size == 0) {
        /* initial values */
        cache[0] = (seq_t)1;
        if (up_to >= 1)
            cache[1] = (seq_t)0;
        cache_size = (up_to >= 1) ? 2 : 1;
    } else if (cache_size == 1) {
        /* we have only a(0) so far */
        cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* now fill from cache_size up to up_to */
    for (size_t i = cache_size; i <= up_to; ++i) {
        seq_t a_prev  = cache[i - 1]; /* a(i-1)   */
        seq_t a_prev2 = cache[i - 2]; /* a(i-2)   */
        seq_t val = a_prev;           /* a(i-1) */
        val += a_prev2;               /* +a(i-2) */
        val += a_prev2;               /* +a(i-2) again (multiply by 2) */
        val += (seq_t)1;              /* +1 */
        cache[i] = val;
    }
    cache_size = up_to + 1;
}

seq_t a(idx_t n)
{
    /* In case n == 0 or 1, we still go through extend_cache which initializes.
     * For clarity, directly handle small cases to avoid unnecessary allocation
     * in a very rare scenario where n==0 and cache==NULL. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    extend_cache(n);
    return cache[(size_t)n];
}
