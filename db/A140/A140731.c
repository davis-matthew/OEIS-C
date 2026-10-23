/*
 * A140731  Primes congruent to 67 or 143 mod 210.
 * https://oeis.org/A140731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes congruent to 67 or 143 mod 210
 * Range:     u8 n <= 1, u16 n <= 217, u32 n <= 273, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t current_k = 0;
    static bool next_is_67 = true;

    while (cache_len < n) {
        seq_t cand;
        if (next_is_67) {
            /* candidate = 67 + 210 * current_k */
            cand = (seq_t)67 + (seq_t)210 * current_k;
            next_is_67 = false;
        } else {
            /* candidate = 143 + 210 * current_k */
            cand = (seq_t)143 + (seq_t)210 * current_k;
            next_is_67 = true;
            current_k++;
        }

        if (is_prime_big(cand))
            append_candidate(&cache, &cache_len, &cache_cap, cand);
    }

    return cache[n - 1];
}
