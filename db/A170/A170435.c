/*
 * A170435  Number of reduced words of length n in Coxeter group on 42
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A170435 – Number of reduced words of length n in a Coxeter
   group on 42 generators with relations S_i^2 = (S_i S_j)^44 = I.
   The sequence satisfies the linear recurrence derived from its
   generating function

       A(t) = (t^44 + 2*t^43 + ⋯ + 2*t^2 + 2*t + 1) /
              (1 - 40*t - 40*t^2 - ⋯ - 40*t^43 + 820*t^44)

   i.e.

       a(n) = 40*a(n-1) + 40*a(n-2) + ⋯ + 40*a(n-43)
              – 820*a(n-44) + p_n   (n ≥ 0)

   where the inhomogeneous term p_n equals 1 for n = 0 or 44 and
   2 for 1 ≤ n ≤ 43, and 0 otherwise.  All negative indices are
   treated as 0.  The following function memoises all values up to
   the current request and computes each new term via the recurrence.
*/

#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *cache = NULL;     /* memoised a(0)…a(k) */
static size_t cache_len = 0;     /* number of valid entries */

static void ensure_cache(idx_t n)
{
    /* nothing to do if already computed */
    if (n < cache_len) return;

    /* grow the cache to size n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)    /* allocation failed – abort */
        abort();
    cache = tmp;

    /* initialise the very first term a(0) */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_len = 1;
    }

    /* compute missing terms from cache_len up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        /* inhomogeneous part p_i */
        seq_t p;
        if (i == 0 || i == 44)
            p = (seq_t)1;
        else if (i <= 43)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* start with the inhomogeneous term */
        seq_t sum = p;

        /* add 40 * a(i-j) for j = 1 … 43 */
        for (idx_t j = 1; j <= 43; ++j) {
            if (i >= j) {
                sum += (seq_t)40 * cache[i - j];
            } else {
                /* a negative index contributes 0 – skip */
                break;
            }
        }

        /* subtract 820 * a(i-44) if i ≥ 44 */
        if (i >= 44) {
            sum -= (seq_t)820 * cache[i - 44];
        }

        cache[i] = sum;
    }

    cache_len = (size_t)(n + 1);
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
