/*
 * A117076  Prime numbers with more even digits than odd digits.
 * https://oeis.org/A117076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes with more even digits than odd digits (OEIS A117076). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 0 */

    /* Static cache of found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* Candidate to be tested for primality. */
    static seq_t next_candidate = (seq_t)2;
    static bool initialized = false;

    /* Ensure cache initialization only once (C++ static initialization). */
    if (!initialized) {
        initialized = true;
    }

    /* Grow cache as needed. */
    if (cache_size < n) {
        while (cache_size < n) {
            /* Find next prime starting from next_candidate. */
            seq_t cand = next_candidate;
            while (!is_prime_big(cand)) {
                cand = cand + (seq_t)1;
            }

            /* Count digits of the prime. */
            unsigned char cnt[10];
            digit_counts(cand, cnt);
            int even = cnt[0] + cnt[2] + cnt[4] + cnt[6] + cnt[8];
            int odd  = cnt[1] + cnt[3] + cnt[5] + cnt[7] + cnt[9];

            if (even > odd) {
                /* Store in cache. */
                if (cache_size + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_size++] = cand;
            }

            /* Set next_candidate to the immediate successor of the found prime. */
            next_candidate = cand + (seq_t)1;
        }
    }

    return cache[n - 1];
}
