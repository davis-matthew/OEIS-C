/*
 * A212858  Number of 5 X n arrays with rows being permutations of 0..n-1 and
 *          no column j greater than column j-1 in all rows.
 * https://oeis.org/A212858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n-1) + Sum_{s=1..n-1} a(s) * (-1)^(n-s-1) *
 *            binomial(n,s)^5 Initial values: a(0)=1, a(1)=1 Recurrence used
 *            for all n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, seq_alloc_new_array, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_pow.h"
#include "../utils/binomial.h"

static seq_t *cache = NULL;      /* cache of computed a(n) values */
static size_t cache_cap = 0;     /* capacity of cache array */
static size_t cache_len = 0;     /* number of values stored (cache_len = highest index + 1) */

/* Helper: compute binomial coefficient nCk multiplicatively (k <= n) */
static seq_t binom_mult(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = (result * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Resize cache if necessary */
    if (cache == NULL) {
        cache_cap = n + 1 > 1 ? n + 1 : 1;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    } else if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        /* copy old values */
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t k = (idx_t)cache_len; k <= n; ++k) {
        seq_t pos = 0;
        seq_t neg = 0;

        /* (-1)^(k-1) part */
        if (((k - 1) & 1) == 0) pos += (seq_t)1;
        else neg += (seq_t)1;

        /* Sum over s = 1 .. k-1 */
        seq_t binom = (seq_t)1;           /* binom(k,0) */
        for (idx_t s = 1; s < k; ++s) {
            /* Update binom to binom(k, s) */
            binom = (binom * (seq_t)(k - s + 1)) / (seq_t)s;
            seq_t binom_pow5 = seq_pow(binom, (idx_t)5);
            seq_t prod = cache[s] * binom_pow5;

            if (((k - s - 1) & 1) == 0)
                pos += prod;
            else
                neg += prod;
        }

        /* Result: pos - neg (pos >= neg for this recurrence) */
        cache[k] = pos - neg;
    }

    cache_len = n + 1;
    return cache[n];
}
