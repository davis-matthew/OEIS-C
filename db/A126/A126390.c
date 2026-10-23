/*
 * A126390  a(n) = Sum_{i=0..n} 2^i*B(i)*binomial(n,i) where B(n) = Bell
 *          numbers A000110(n).
 * https://oeis.org/A126390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} 2^i * B(i) * binomial(n,i) Recurrence used:
 *            a(0)=1; a(n)=a(n-1)+Sum_{k=1..n} binomial(n-1,k-1)*2^k*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp); */

static seq_t *cache = NULL;          /* array of previously computed a(n) */
static idx_t cache_size = 0;         /* number of elements filled in cache */
static idx_t cache_cap   = 0;         /* allocated capacity of cache */

/* ensure cache has capacity at least given amount */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need) return;
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) abort();   /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* compute values up to n if not already cached */
    if (n >= cache_size) {
        ensure_cache_capacity(n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;                /* a(0)=1 */
                continue;
            }
            seq_t sum = 0;
            seq_t binom = 1;     /* C(i-1,0) */
            seq_t pow2k = 2;       /* 2^1 */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = binom(i-1,k-1) * 2^k * a(i-k) */
                seq_t term = binom * pow2k;
                term = term * cache[i - k];
                sum += term;

                /* update binomial for next k (if any) */
                if (k < i) {
                    /* binom(i-1,k) = binom(i-1,k-1) * (i-1-(k-1)) / k */
                    binom = binom * (i - k);
                    binom = binom / k;
                }

                /* advance power of two */
                pow2k = pow2k * 2;
            }
            cache[i] = cache[i - 1] + sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
