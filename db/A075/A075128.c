/*
 * A075128  Binomial transform of generalized tetranacci numbers A073817:
 *          a(n)=Sum((-1)^k Binomial(n,k)*A073817(k),(k=0,..,n)).
 * https://oeis.org/A075128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - 2*a(n-3) + 3*a(n-4) for n >= 4,
 *            a(0) = 4, a(1) = 3, a(2) = 5, a(3) = 3 (equivalently: a(n) =
 *            3*a(n-1) + 3*a(n-4) - 2*a(n-2) - 2*a(n-3))
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;    /* cache of computed terms      */
static idx_t  cache_size = 0;      /* number of terms stored       */

/* Ensure that seq_cache contains terms up to and including index n.   */
static void ensure_cache(idx_t n)
{
    if (seq_cache == NULL) {
        /* initialise cache with the first four terms */
        cache_size = 4;
        seq_cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        seq_cache[0] = 4;
        seq_cache[1] = 3;
        seq_cache[2] = 5;
        seq_cache[3] = 3;
    }
    if (n < cache_size)
        return;

    /* grow the cache to store terms up to n */
    seq_t *tmp = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    if (!tmp) { /* allocation failed – cannot recover reliably   */
        /* exit or abort; here we simply return (undefined behaviour) */
        return;
    }
    seq_cache = tmp;

    /* compute new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 3 * seq_cache[i - 1];
        term += 3 * seq_cache[i - 4];
        term -= 2 * seq_cache[i - 2];
        term -= 2 * seq_cache[i - 3];
        seq_cache[i] = term;
    }
    cache_size = n + 1;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return seq_cache[n];
}
