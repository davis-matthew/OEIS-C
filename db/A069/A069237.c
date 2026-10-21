/*
 * A069237  Composite numbers k such that tau(k) divides phi(k), where tau(k)
 *          is the number of divisors of k and phi(k) the Euler totient
 *          function.
 * https://oeis.org/A069237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, is_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Composite numbers k such that tau(k) divides phi(k). Computed by scanning integers. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/is_prime.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t candidate = 3;   /* start after 3, since 3 is prime */

    if (cache == NULL) {
        cache_cap = 64;
        cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (cache_len < (size_t)n) {
        ++candidate;
        if (is_prime(candidate)) continue;       /* skip primes */

        uint64_t tau = divisor_count_u64(candidate);
        seq_t phi_val = phi(candidate);
        uint64_t phi_u64 = (uint64_t)phi_val;   /* phi ≤ candidate-1 < 2^64 */

        if (phi_u64 % tau == 0) {               /* condition holds */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[(size_t)(n - 1)];
}
