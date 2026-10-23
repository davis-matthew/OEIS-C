/*
 * A023237  Primes p such that 10*p + 1 is also prime.
 * https://oeis.org/A023237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 128, u32 n <= 139, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 10*p + 1 is also prime (search based implementation) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* below offset – not used */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 2;          /* first prime to test */

    /* grow cache when needed */
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure → undefined */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* skip even numbers except 2 */
        if (next_candidate > 2 && !(next_candidate & 1U))
            next_candidate++;

        /* test next_candidate */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t q = (seq_t)(10ULL * next_candidate + 1ULL);
            if (is_prime_big(q)) {
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }

        /* move to next candidate */
        next_candidate++;
        /* the loop will grow cache if still full */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    return cache[n - 1];
}
