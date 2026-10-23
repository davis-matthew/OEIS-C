/*
 * A142786  Primes congruent to 7 mod 60.
 * https://oeis.org/A142786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime ≡ 7 (mod 60)
 * Range:     u8 n <= 3, u16 n <= 310, u32 n <= 373, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Memoization: cache of primes found so far */
static seq_t *primes_cache = NULL;
static idx_t primes_cap = 0;
static idx_t primes_len = 0;

/* Grow the cache to at least needed size */
static void grow_cache(idx_t needed) {
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < (size_t)needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(primes_cache, new_cap * sizeof(seq_t));
    primes_cache = tmp;
    primes_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* not used per OEIS offset, guard only */
    while (primes_len < n) {
        seq_t candidate;
        if (primes_len == 0) {
            candidate = 7;
        } else {
            candidate = primes_cache[primes_len - 1] + 60;
        }
        /* Search until next prime ≡ 7 (mod 60) */
        for (;;) {
            if (is_prime_big(candidate)) {
                grow_cache(primes_len + 1);
                primes_cache[primes_len++] = candidate;
                break;
            }
            candidate += 60;
        }
    }
    return primes_cache[n - 1];
}
