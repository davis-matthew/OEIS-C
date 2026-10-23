/*
 * A139994  Primes of the form 24x^2+35y^2.
 * https://oeis.org/A139994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 202, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 24x^2+35y^2 (OEIS A139994).
   The function returns the n‑th such prime by testing candidates in
   increasing order and verifying that they are prime and congruent
   to one of {59,131,251,299,419,731} modulo 840. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 2;
    static uint8_t allowed[840];
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        for (int i = 0; i < 840; ++i) allowed[i] = 0;
        int residues[6] = {59, 131, 251, 299, 419, 731};
        for (int i = 0; i < 6; ++i) allowed[residues[i]] = 1;
        cache_cap = 1024;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        /* Search for the next candidate */
        while (true) {
            idx_t num = next_candidate++;
            if (!allowed[num % 840]) continue;          // Not one of the allowed residues
            if (is_prime_small((seq_t)num)) {           // Prime test
                if (cache_len == cache_cap) {           // Grow cache if needed
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)num;
                break;                                 // Found next prime in the sequence
            }
        }
    }

    return cache[n - 1];
}
