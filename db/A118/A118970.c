/*
 * A118970  a(n) = 3*binomial(5n+2,n)/(4n+3).
 * https://oeis.org/A118970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*binomial(5*n+2,n)/(4*n+3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n) = 5*(5n+1)*(5n+2)*(5n-2)*(5n-1)*a(n-1) /
                    (8*n*(4n+1)*(2n+1)*(4n+3))  (n ≥ 1) */
/* The function memoises values in a static array so repeated calls are cheap. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* memoisation array */
static idx_t cache_len = 0;      /* number of values stored (including a(0)) */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* ensure the cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_len = 1;
    } else if (cache_cap <= (size_t)n) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    ensure_cache(n);

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence coefficients */
        seq_t num = (seq_t)5 *
                    ((seq_t)(5 * i + 1) * (seq_t)(5 * i + 2));
        seq_t tmp = (seq_t)(5 * i - 2) * (seq_t)(5 * i - 1);
        num *= tmp;   /* num = 5*(5i+1)*(5i+2)*(5i-2)*(5i-1) */

        seq_t den = (seq_t)8 * (seq_t)i *
                    (seq_t)(4 * i + 1) *
                    (seq_t)(2 * i + 1) *
                    (seq_t)(4 * i + 3);  /* den = 8*i*(4i+1)*(2i+1)*(4i+3) */

        /* reduce with previous value a(i-1) to keep numbers small */
        seq_t prev = cache[i - 1];

        seq_t g = gcd(prev, den);
        prev /= g;
        den /= g;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;   /* after reductions den should be 1 */

        /* compute a(i) = prev * num / den (den == 1 here) */
        seq_t result = prev * num;
        cache[i] = result;
    }

    cache_len = (n + 1);  /* now we have values up to n */
    return cache[n];
}
