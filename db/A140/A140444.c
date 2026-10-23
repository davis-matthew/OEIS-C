/*
 * A140444  Primes congruent to 1 (mod 14).
 * https://oeis.org/A140444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 1 (mod 14), offset 1
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;  /* undefined; callers should not use n = 0 */
    }

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_candidate = 29;  /* first prime ≡ 1 (mod 14) */

    /* generate terms until the cache covers the requested index */
    while (cache_len < n) {
        /* find the next prime ≡ 1 (mod 14) */
        while (!is_prime((idx_t)next_candidate)) {
            next_candidate += 14;
        }

        /* store the found prime */
        ensure_capacity(cache_len + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));
        cache[cache_len++] = next_candidate;

        /* advance to the following candidate */
        next_candidate += 14;
    }

    return cache[n - 1];
}
