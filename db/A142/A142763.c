/*
 * A142763  Primes congruent to 36 mod 59.
 * https://oeis.org/A142763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 36 modulo 59
 * Range:     u8 none, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;      /* next number to test for primality */

static void generate_upto(size_t target)
{
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return;          /* allocation failure – caller may handle */
    }

    while (cache_len < target) {
        uint64_t cand = next_candidate;
        for (;;) {
            if (is_prime((idx_t)cand) && (cand % 59U == 36U)) {
                /* store the prime */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (tmp == NULL) return;    /* allocation failure */
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)cand;
                next_candidate = cand + 1;
                break;
            }
            cand++;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    generate_upto((size_t)n);
    return cache[(size_t)(n - 1)];
}
