/*
 * A118935  E.g.f.: A(x) = exp( Sum_{n>=0} x^(4^n)/4^((4^n-1)/3) ).
 * https://oeis.org/A118935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} n!/[k!*(n-4k)!*4^k] * a(k), with a(0)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* array of previously computed a(n) */
static idx_t cache_size = 0;     /* number of entries in cache */

/* Ensure that cache[0..n] are computed.  The loop is performed only once
   for each new index, and results are reused for all later calls. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    size_t new_size = (size_t)(n + 1);
    /* realloc is C++-friendly with the cast below. */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                   /* allocation failed */
    cache = tmp;

    /* Compute new entries, starting from the current cache_size */
    for (idx_t m = cache_size; m <= n; ++m)
    {
        if (m == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum  = (seq_t)0;          /* accumulates a(m) */
        seq_t comb = (seq_t)1;          /* combinatorial factor
                                          comb(k-1) = (m)!/((k-1)!*(m-4(k-1))!*4^(k-1)) */

        /* k = 0 term */
        sum += cache[0];

        idx_t maxk = m / 4;
        for (idx_t k = 1; k <= maxk; ++k)
        {
            /* Update comb from k-1 to k:
               multiply by (m-4k+1)...(m-4k+4) and divide by 4*k. */
            comb *= (seq_t)(m - 4 * k + 1);
            comb *= (seq_t)(m - 4 * k + 2);
            comb *= (seq_t)(m - 4 * k + 3);
            comb *= (seq_t)(m - 4 * k + 4);
            comb /= (seq_t)(4 * k);

            sum += comb * cache[k];
        }

        cache[m] = sum;
    }

    cache_size = new_size;
}

/* Public entry point for the sequence.  It works for any order of calls
   and uses memoisation to keep the cost O(n) for each new maximum n
   encountered. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
