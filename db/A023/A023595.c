/*
 * A023595  a(n) = sum of exponents in prime-power factorization of
 *          2*prime(n)+1.
 * https://oeis.org/A023595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001222(A072055(n)) where A072055(n) = 2*prime(n)+1 and
 *            A001222 counts prime factors with multiplicity (Ω)
 * Range:     u8 n <= 32, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_prime_factors, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n) {
    /* cache for already computed values */
    static seq_t *cache = NULL;      /* cache[i] holds a(i+1) */
    static size_t cache_size = 0;    /* number of entries currently stored */

    if (n == 0) {
        return 0;                    /* offset is 1, but guard against 0 */
    }

    /* enlarge cache if necessary */
    if ((idx_t)n > (idx_t)cache_size) {
        size_t new_size = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* out of memory – in practice this should never happen */
            return 0;
        }
        cache = new_cache;

        /* compute missing terms */
        for (size_t i = cache_size; i < n; ++i) {
            idx_t k = (idx_t)(i + 1);          /* k = n for the k‑th term */
            uint32_t p = get_prime(k);        /* the k‑th prime (1‑based) */

            seq_t m = (seq_t)2 * (seq_t)p + 1;
            seq_t omega = count_prime_factors(m);

            cache[i] = omega;
        }
        cache_size = n;
    }

    return cache[n - 1];
}
