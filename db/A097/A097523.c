/*
 * A097523  a(n) = least k such that k - prime(n) and k + prime(n) are both
 *          prime.
 * https://oeis.org/A097523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that k - prime(n) and k + prime(n) are both
 *            prime
 * Range:     u8 n <= 30, u16 n <= 2957, u32 n <= 3215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by straightforward search. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term.  n must be ≥ 1.  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cache slots (index 0 unused) */

    if (n == 0) return 0;  /* for safety; not part of the defined sequence */

    /* ensure the cache can hold a[n] */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;              /* store indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                   /* allocation failure */
        /* zero‑initialize newly added slots */
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* return cached value if available */
    if (cache[n] != 0) return cache[n];

    /* find the n‑th prime */
    idx_t p = get_prime(n);          /* p fits in idx_t */

    /* search for the least k ≥ p+2 satisfying the condition */
    seq_t k = (seq_t)p + 2;
    for (;;) {
        seq_t km = k - (seq_t)p;     /* k - p */
        seq_t kp = k + (seq_t)p;     /* k + p */
        if (is_prime_big(km) && is_prime_big(kp)) {
            cache[n] = k;          /* store and return */
            return k;
        }
        ++k;
    }
}
