/*
 * A006567  Emirps (primes whose reversal is a different prime).
 * https://oeis.org/A006567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 979, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Generate emirps: primes p where reverse(p) is prime and reverse(p) != p. */

static seq_t *cache = NULL;
static idx_t cache_count = 0;
static idx_t cache_capacity = 0;
static seq_t next_candidate = 13;

static seq_t reverse_number(seq_t num) {
    seq_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + (num % 10);
        num /= 10;
    }
    return reversed;
}

seq_t a(idx_t n) {
    while (cache_count < n) {
        if (is_prime((uint64_t)next_candidate)) {
            seq_t rev = reverse_number(next_candidate);
            if (rev != next_candidate && is_prime((uint64_t)rev)) {
                if (cache_count >= cache_capacity) {
                    cache_capacity = cache_capacity == 0 ? 100 : cache_capacity * 2;
                    cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
                }
                cache[cache_count++] = next_candidate;
            }
        }
        next_candidate += 2;
    }
    
    return cache[n - 1];
}
