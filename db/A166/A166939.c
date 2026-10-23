/*
 * A166939  Numerators of partial sums (n+1)/n (not sorted).
 * https://oeis.org/A166939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the partial sum S_n = n + H_n,
   where H_n = sum_{k=1}^{n} 1/k is the nth harmonic number
   reduced to lowest terms.  The sequence is OEIS A166939. */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *num_cache = NULL;   /* numerators of H_n    */
static seq_t *den_cache = NULL;   /* denominators of H_n  */
static seq_t *a_cache   = NULL;   /* numerators of S_n    */
static size_t cache_size = 0;      /* number of elements stored (=max index + 1) */

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Protect against an unexpectedly small n (offset starts at 1). */
    if (n == 0)
        return 0;

    /* Initialise caches for the base case H_0 = 0/1, S_0 = 0. */
    if (cache_size == 0) {
        cache_size = 1;
        num_cache = (seq_t *)malloc(sizeof(seq_t));
        den_cache = (seq_t *)malloc(sizeof(seq_t));
        a_cache   = (seq_t *)malloc(sizeof(seq_t));
        if (!num_cache || !den_cache || !a_cache)
            return 0;                      /* allocation failure */

        num_cache[0] = 0;                  /* H_0 */
        den_cache[0] = 1;
        a_cache[0]   = 0;                  /* S_0 (unused) */
    }

    /* Extend the cache up to the requested index n. */
    if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        num_cache = (seq_t *)realloc(num_cache, new_size * sizeof(seq_t));
        den_cache = (seq_t *)realloc(den_cache, new_size * sizeof(seq_t));
        a_cache   = (seq_t *)realloc(a_cache,   new_size * sizeof(seq_t));
        if (!num_cache || !den_cache || !a_cache)
            return 0;                      /* allocation failure */

        for (size_t idx = cache_size; idx < new_size; ++idx) {
            idx_t i = (idx_t)idx;          /* current harmonic index */

            seq_t p_prev = num_cache[idx - 1];
            seq_t q_prev = den_cache[idx - 1];

            /* H_i = H_{i-1} + 1/i = (p_prev * i + q_prev) / (q_prev * i) */
            seq_t p = (seq_t)i * p_prev + q_prev;
            seq_t q = q_prev * (seq_t)i;

            seq_t g = gcd(p, q);
            p /= g;
            q /= g;
            num_cache[idx] = p;
            den_cache[idx] = q;

            /* S_i = i + H_i = (i * q + p) / q  (numerator already reduced) */
            a_cache[idx] = (seq_t)i * q + p;
        }
        cache_size = new_size;
    }

    return a_cache[(size_t)n];
}
