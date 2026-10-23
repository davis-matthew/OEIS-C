/*
 * A121240  Numerator of sum_{k=1..n} 1/2^prime(k).
 * https://oeis.org/A121240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[ Sum_{k=1..n} 1/2^Prime[k] ]
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=2^(Prime[n]-Prime[n-1])*a(n-1)+1 for n>1 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;     /* array of computed values */
static idx_t cache_size = 0;    /* number of values stored (indices 1..cache_size) */

static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* grow storage */
    size_t new_cap = (size_t)(n + 1);           /* capacity for indices 0..n */
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache)
        return;                                 /* allocation failed – abort safely */

    /* compute missing entries */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t prev      = cache[i - 1];
            idx_t p_curr    = (idx_t)get_prime(i);
            idx_t p_prev    = (idx_t)get_prime(i - 1);
            seq_t shift     = pow2(p_curr - p_prev);   /* 2^(p_curr-p_prev) */
            cache[i] = prev * shift + (seq_t)1;
        }
    }
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1)        /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n];
}
