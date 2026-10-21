/*
 * A226518  Irregular triangle read by rows: T(n,k) = Sum_{i=0..k}
 *          Legendre(i,prime(n)).
 * https://oeis.org/A226518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the nth element of the irregular triangle read by rows:
 *            T(n,k) = sum_{i=0..k} (i | prime(n)), where (·|p) is the
 *            Legendre symbol. The sequence enumerates these partial sums in
 *            row-major order
 * Range:     i8 n <= 9371, i16 n <= 9371, i32 n <= 9371, i64 n <= 9371,
 *            i128 n <= 9371, big any n
 * Helpers:   jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Static data structures for memoisation */
    static seq_t *cache   = NULL;   /* stores flattened triangle */
    static size_t cache_len = 0;    /* number of elements currently stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    static uint64_t *primes   = NULL;   /* list of primes in growing order */
    static size_t primes_len   = 0;     /* number of primes stored */
    static size_t primes_cap   = 0;     /* allocated capacity of primes */

    if (n == 0)                 /* by definition the sequence starts at n=1 */
        return 0;

    /* Extend the cache until it covers the requested index n */
    while (cache_len < (size_t)n) {
        /* --- generate the next prime --- */
        uint64_t p;
        if (primes_len == 0) {
            /* first prime is 2 */
            p = 2;
            if (primes_cap == 0) {
                primes_cap = 4;
                primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
            } else if (primes_len == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = p;
        } else {
            p = next_prime(primes[primes_len - 1]);
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap << 1 : 4;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_len++] = p;
        }

        /* --- build the partial sums for the current prime row --- */
        seq_t sum = 0;
        for (uint64_t i = 0; i < p; ++i) {
            int leg = jacobi_symbol(i, p);          /* Legendre(i|p) */
            sum += (seq_t)leg;                      /* cumulative sum (seq_t handles ±1,0) */

            /* Store the new value into the cache, resizing if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = sum;
        }
    }

    /* Return the n‑th term (1‑based index). */
    return cache[n - 1];
}
