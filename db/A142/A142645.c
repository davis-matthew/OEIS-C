/*
 * A142645  Primes congruent to 11 mod 56.
 * https://oeis.org/A142645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A142645 – primes ≡ 11 (mod 56)
    a(n) = the n‑th prime p such that p ≡ 11 (mod 56)   (offset 1)
    The function uses a tame on‑demand search with a small cache.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"    /* bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    /* static cache of found primes in the congruence class */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count    = 0;
    static uint64_t candidate = 11;   /* first number ≡ 11 (mod 56) */

    if (n == 0) return 0;  /* offset is 1 – undefined for 0 */

    /* grow cache as needed */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* collect primes until we have at least n of them */
    while (count < (size_t)n) {
        if (is_prime((idx_t)candidate)) {
            /* store the prime, enlarging the cache if required */
            if (count == capacity) {
                capacity <<= 1;
                cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            }
            cache[count++] = (seq_t)candidate;
        }
        candidate += 56;   /* next number ≡ 11 (mod 56) */
    }

    return cache[n - 1];
}
