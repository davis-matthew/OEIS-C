/*
 * A142171  Primes congruent to 20 mod 39.
 * https://oeis.org/A142171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 275, u32 n <= 89, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes congruent to 20 mod 39 (OEIS A142171) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache of found primes and state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t last_k = 0;          /* next k to try: candidate = 20 + 39*last_k */

    /* grow cache until we have at least n primes */
    while (cache_len < (size_t)n) {
        idx_t cand_num = 20 + 39 * last_k;
        last_k++;

        /* convert candidate to seq_t via seq_from_str */
        char buf[32];
        snprintf(buf, sizeof(buf), "%llu", (unsigned long long)cand_num);
        seq_t cand = seq_from_str(buf);

        if (is_prime_big(cand)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();     /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}
