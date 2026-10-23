/*
 * A159933  INVERTi transform of d(n), A000005.
 * https://oeis.org/A159933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 79, i32 n <= 153, i64 n <= 304,
 *            i128 n <= 610, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159933: Invert i transform of d(n) (divisor count)
   a(1) = 1
   a(n) = d(n) - Σ_{k=1}^{n-1} a(k) * d(n-k)   (n ≥ 2)           */

#include <stdlib.h>
#include <stddef.h>

/* divisor counts d(1)..d(n)  */
static idx_t *divs = NULL;
static size_t div_cap = 0;

/* cache for a(1)..a(n)      */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;   /* largest index computed so far */

/* Compute divisor counts up to n (inclusive).  Efficient O(n log n) sieve. */
static void compute_divs(idx_t n)
{
    if (div_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)n + 1;
        idx_t *tmp = (idx_t *)realloc(divs, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        divs = tmp;
        div_cap = new_cap;
    }
    /* initialise to zero for indices 1..n */
    for (size_t i = 1; i <= (size_t)n; ++i) divs[i] = 0;
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++divs[(size_t)j];
}

/* expand cache if needed up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (size_t)(n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    compute_divs(n);
    ensure_cache(n);

    /* compute a(m) for all m that have not yet been computed */
    for (size_t m = cache_size + 1; m <= (size_t)n; ++m) {
        if (m == 1) {
            cache[m] = (seq_t)1;
        } else {
            seq_t sum = 0;
            for (size_t k = 1; k < m; ++k) {
                sum += cache[k] * (seq_t)divs[m - k];
            }
            cache[m] = (seq_t)divs[m] - sum;
        }
    }
    cache_size = (size_t)n;
    return cache[n];
}
