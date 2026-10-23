/*
 * A142266  Primes congruent to 17 mod 43.
 * https://oeis.org/A142266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime that is congruent to 17 modulo 43
 * Range:     u8 n <= 2, u16 n <= 157, u32 n <= 158, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t count = 0;
    static size_t cap = 0;
    static uint64_t last_candidate = 0;

    if (cap == 0) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (count < n) {
        uint64_t candidate = (last_candidate == 0) ? 17 : last_candidate + 43;
        if (is_prime_big((seq_t)candidate)) {
            if (count == cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                memcpy(tmp, cache, count * sizeof(seq_t));
                free(cache);
                cache = tmp;
                cap = new_cap;
            }
            cache[count++] = (seq_t)candidate;
        }
        last_candidate = candidate;
    }
    return cache[n - 1];
}
