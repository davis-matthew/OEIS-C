/*
 * A069914  a(n) = Sum_{d|n} (d-1)*sigma(n/d).
 * https://oeis.org/A069914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d-1) * sigma(n/d)
 * Range:     u8 n <= 35, u16 n <= 1079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sigma(k) is the sum of all positive divisors of k) */

#include <stdint.h>
#include <stdlib.h>

/* static cache for sigma values up to sigma_max */
static seq_t *sigma_cache = NULL;
static idx_t sigma_max   = 0;

/* ensure sigma_cache contains sigma(i) for all i <= n */
static void ensure_sigma(idx_t n)
{
    if (n <= sigma_max) return;                /* already computed */

    /* allocate a new array of size (n+1) */
    seq_t *new_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) return;                    /* out‑of‑memory guard */

    /* initialise all entries to zero (works for builtin and big‑int types) */
    for (idx_t i = 0; i <= n; ++i)
        new_cache[i] = 0;

    /* sieve to compute sigma values */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            new_cache[j] += i;

    /* discard old cache if it exists */
    if (sigma_cache)
        free(sigma_cache);

    sigma_cache = new_cache;
    sigma_max   = n;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safeguard, although n>=1 by definition */

    ensure_sigma(n);

    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            result += (seq_t)(d - 1) * sigma_cache[q];
            if (d != q)              /* avoid double counting for square roots */
                result += (seq_t)(q - 1) * sigma_cache[d];
        }
    }
    return result;
}
