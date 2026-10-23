/*
 * A141872  Primes congruent to 7 mod 19.
 * https://oeis.org/A141872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 7 mod 19
 * Range:     u8 n <= 3, u16 n <= 369, u32 n <= 370, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t k = 0;           /* candidate index, starts at 0 -> 7 */

    while (cache_len < n) {
        seq_t cand = ((seq_t)19 * (seq_t)k) + (seq_t)7;
        if (is_prime_big(cand)) {
            append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
        k++;                     /* move to next candidate */
    }

    return cache[n - 1];
}
