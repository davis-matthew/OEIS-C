/*
 * A175118  a(1)=2. a(n) = the smallest prime p > a(n-1) such that p-a(n-1)+1
 *          is composite.
 * https://oeis.org/A175118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n>1, a(n)=min{p prime > a(n‑1) : p-a(n‑1)+1 is
 *            composite}
 * Range:     u8 n <= 9, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_sz = 0;

static void ensure_cache(idx_t need)
{
    if (need == 0) return;
    if (cache == NULL) {
        cache_cap = need;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (need > cache_cap) {
        cache_cap = need;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined below offset */

    ensure_cache(n);

    if (cache_sz == 0) {                    /* initialise first term */
        cache[0] = (seq_t)2;
        cache_sz = 1;
    }

    while (cache_sz < n) {                   /* generate terms up to n */
        seq_t prev = cache[cache_sz - 1];
        seq_t candidate = prev + (seq_t)1;

        while (1) {
            if (!is_prime_big(candidate)) {  /* not a prime */
                candidate++;
                continue;
            }
            /* candidate is prime – test the composite condition */
            seq_t diff = candidate - prev + (seq_t)1;
            if (!is_prime_big(diff))         /* composite diff -> accept */
                break;
            candidate++;                     /* look for next prime */
        }

        cache[cache_sz++] = candidate;        /* store the next term */
    }

    return cache[n - 1];
}
