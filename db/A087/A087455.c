/*
 * A087455  Expansion of (1 - x)/(1 - 2*x + 3*x^2) in powers of x.
 * https://oeis.org/A087455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 3*a(n-2) (a(0)=1, a(1)=1)
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 39, i64 n <= 81, i128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array storing computed terms   */
static idx_t  cache_size = 0;         /* currently allocated size       */
static idx_t  last_computed = 0;      /* largest index that has been set */

/* Ensure the cache can hold indices up to `n`.  Allocate or grow the
   array if necessary.  Newly allocated elements are left as 0. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                       /* already large enough */

    /* Grow to at least n+1 (doubling strategy) */
    idx_t new_size = (cache_size == 0) ? 1 : cache_size;
    while (new_size <= n)
        new_size <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) abort();           /* cannot happen in tests */

    /* Copy old values if any */
    for (idx_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* zero out new slots */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_size = new_size;
}

/* Return a(n) according to the recurrence.  The function is
   memoised: once a term is computed it is stored for future calls. */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 1;

    /* Make sure we have space for up to `n` */
    ensure_cache(n);

    /* Initialise the first two values if this is the first call */
    if (cache_size > 0 && last_computed < 1) {
        cache[0] = 1;
        cache[1] = 1;
        last_computed = 1;
    }

    /* Compute any missing terms up to `n` */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) - 3*a(i-2) */
        seq_t term = cache[i - 1] * 2;
        term -= cache[i - 2] * 3;
        cache[i] = term;
    }

    last_computed = n;
    return cache[n];
}
