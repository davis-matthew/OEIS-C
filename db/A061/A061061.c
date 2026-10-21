/*
 * A061061  Maximal number of 132 patterns in a permutation of 1,2,...,n.
 * https://oeis.org/A061061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = max_{1 <= k < n} (a(k) + k * C(n-k, 2))
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 0, a(2) = 0, ...  (recurrence with memoisation)   */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;          /* cache[i] holds a(i+1)          */
static size_t cache_len = 0;         /* number of stored values so far */

/* Returns the maximal number of 132 patterns in a permutation of 1..n. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* If we already have the value, return it immediately. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Expand the cache to hold up to n entries. */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n;
    seq_t *tmp = NULL;

#ifdef __cplusplus
    /* In C++ build use new to construct objects. */
    tmp = new(std::nothrow) seq_t[new_len];
    if (!tmp) return 0; /* allocation failure */

    /* Copy the old values into the new array. */
    for (size_t i = 0; i < old_len; ++i)
        tmp[i] = cache[i];

    /* Delete the old array. */
    delete[] cache;
#else
    /* In C build use realloc (which copies data). */
    tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
#endif

    /* Initialise new slots to zero. */
    for (size_t i = old_len; i < new_len; ++i)
        tmp[i] = 0;

    cache     = tmp;
    cache_len = new_len;

    /* Compute missing values using the recurrence. */
    for (idx_t i = (idx_t)(old_len + 1); i <= n; ++i) {
        seq_t best = 0;
        for (idx_t k = 1; k < i; ++k) {
            idx_t rem  = i - k;                               /* n-k  */
            seq_t binom = ((seq_t)rem * (rem - 1)) / 2;        /* C(rem,2) */
            seq_t cand  = cache[k - 1] + ((seq_t)k) * binom;   /* a(k)+k*C ... */
            if (cand > best) best = cand;
        }
        cache[i - 1] = best;
    }

    return cache[n - 1];
}
