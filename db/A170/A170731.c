/*
 * A170731  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170731: number of reduced words of length n in a Coxeter
   group with 50 generators S_i and relations (S_i)^2 = (S_i S_j)^50 = I.
   Generating function G(t) = (t^50 + 2*t^49 + … + 2*t + 1) /
                             (1176*t^50 - 48*t^49 - … - 48*t + 1).
   Recurrence:
       a(0) = 1
       for 1 ≤ n ≤ 49:
           a(n) = 48 * Σ_{k=1}^{n}  a(n-k) + 2
       a(50) = 48 * Σ_{k=1}^{49} a(50-k) - 1176 * a(0) + 1
       for n ≥ 51:
           a(n) = 48 * Σ_{k=1}^{49} a(n-k) - 1176 * a(n-50) */
#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed a(i) */
static size_t cache_size = 0;    /* allocated size of cache[]   */
static idx_t cached_until = (idx_t)-1;   /* last index we have computed */

/* Ensure that cache can hold indices up to n inclusive. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_size)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
        abort();   /* out of memory */
    cache   = tmp;
    cache_size = needed;
}

/* Compute a(n).  Cached values are reused for later calls. */
seq_t a(idx_t n)
{
    /* If we have not yet initialized the cache, create a(0). */
    if (cached_until == (idx_t)-1) {
        ensure_cache(0);
        cache[0] = 1;
        cached_until = 0;
    }

    if (n > cached_until) {
        ensure_cache(n);
        for (idx_t i = cached_until + 1; i <= n; ++i) {
            seq_t sum = 0;
            if (i == 0) {
                cache[i] = 1;          /* a(0) = 1 */
                continue;
            }

            /* Sum of the preceding 1..min(49,i) terms. */
            idx_t lim = i < 49 ? i : 49;
            for (idx_t k = 1; k <= lim; ++k)
                sum += cache[i - k];

            if (i < 50) {
                /* a(i) = 48*sum + 2 */
                cache[i] = 48 * sum + 2;
            } else if (i == 50) {
                /* a(50) = 48*sum - 1176*a(0) + 1 */
                cache[i] = 48 * sum - 1176 * cache[0] + 1;
            } else {
                /* a(i) = 48*sum - 1176*a(i-50) */
                cache[i] = 48 * sum - 1176 * cache[i - 50];
            }
        }
        cached_until = n;
    }

    return cache[n];
}
