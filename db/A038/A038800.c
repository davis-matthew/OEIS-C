/*
 * A038800  Number of primes between 10n and 10n+9.
 * https://oeis.org/A038800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   Count of primes in the interval [10*n, 10*n+9]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function works for any seq_t and idx_t defined by the build,
   using is_prime(idx_t) to test primality. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache the results of previous computations */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of cached elements */

    /* If the value is already cached, return immediately */
    if ((idx_t)cache_size > n) {
        return cache[(size_t)n];
    }

    /* Resize the cache to hold values up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL) {
        /* Allocation failed – return 0 (not expected for the test cases) */
        return (seq_t)0;
    }
    cache = tmp;

    /* Compute missing values from the old cache size up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t count = 0;
        idx_t base = i * (idx_t)10;   /* 10*i  */
        for (idx_t j = 0; j < 10; ++j) {
            if (is_prime(base + j)) {
                ++count;
            }
        }
        cache[(size_t)i] = count;
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
