/*
 * A167448  Smallest number with n distinct positive cubic exponents in its
 *          prime factorization.
 * https://oeis.org/A167448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} prime(k)^{(n-k+1)^3} (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utilities provided by the framework */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/get_prime.h"

/* cache for already computed terms */
static seq_t *cache       = NULL;    /* array of a(i) */
static size_t cache_cap   = 0;       /* allocated size of cache */
static size_t cache_len   = 0;       /* number of terms stored in cache */

/* ensure that cache has at least new_cap elements */
static void reserve_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        abort(); /* unrecoverable allocation failure */
    }
    cache     = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        if (cache_len == 0) {
            reserve_cache(1);
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* ensure cache capacity up to n */
    reserve_cache((size_t)n + 1);

    /* compute missing terms up to n */
    if (cache_len == 0) {          /* first time */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
        seq_t acc = (seq_t)1;                 /* product accumulator */

        for (idx_t k = 1; k <= m; ++k) {
            idx_t exp  = m - k + 1;           /* (n-k+1) */
            idx_t exp3 = exp;
            exp3 *= exp;                      /* exp^2 */
            exp3 *= exp;                      /* exp^3 */

            seq_t pow = seq_pow((seq_t)get_prime(k), exp3);
            acc = seq_mul(acc, pow);
        }
        cache[(size_t)m] = acc;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
