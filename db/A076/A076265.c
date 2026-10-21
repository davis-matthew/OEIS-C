/*
 * A076265  a(n) = Product_{i=1..n} prime(i)^prime(i).
 * https://oeis.org/A076265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Product_{i=1..n} prime(i)^prime(i)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_cache, get_prime, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cache[i] stores a(i+1) */
    static size_t cap   = 0;        /* allocated capacity of cache   */
    static idx_t last_n = 0;        /* highest n for which we have computed */

    if (n < 1)
        return (seq_t)1;            /* undefined for n=0, return 1 as neutral element */

    /* Ensure sufficient space for all computed values up to n */
    ensure_cache(n, &cache, &cap);

    /* Return cached value if already computed */
    if (n <= last_n)
        return cache[n - 1];

    /* Compute any remaining values up to n, storing in cache */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        uint32_t p = get_prime(i);                     /* i-th prime (uint32_t)   */
        seq_t term = seq_pow((seq_t)p, (idx_t)p);      /* p^p as seq_t             */
        seq_t prev = (i == 1) ? (seq_t)1 : cache[i - 2];
        cache[i - 1] = seq_mul(prev, term);            /* a(i) = a(i-1) * p^p      */
    }

    last_n = n;
    return cache[n - 1];
}
