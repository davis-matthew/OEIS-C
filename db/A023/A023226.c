/*
 * A023226  Primes p such that 7*p + 8 is also prime.
 * https://oeis.org/A023226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 420, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 7*p+8 is prime */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next_start = 2;
    while (len < (size_t)n) {
        uint64_t p = next_prime(next_start);
        if (!p) break; /* safety */
        uint64_t val = 7ULL * p + 8ULL;
        if (is_prime_big((seq_t)val)) {
            if (len >= cap) {
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = (seq_t)p;
        }
        next_start = p + 1;
    }
    return cache[n-1];
}
