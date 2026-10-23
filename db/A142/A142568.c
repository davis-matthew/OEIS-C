/*
 * A142568  Primes congruent to 38 mod 53.
 * https://oeis.org/A142568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p ≡ 38 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 103, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t len = 0;
    static idx_t next_k = 0; /* next k for candidate 53*k+38 */

    while (len < n) {
        seq_t cand = (seq_t)53 * (seq_t)next_k + 38;

        if (is_prime_big(cand)) {
            if (len == cap) {
                size_t new_cap = cap == 0 ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failed */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = cand;
        }
        ++next_k;
    }

    return cache[n-1];
}
