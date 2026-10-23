/*
 * A109611  Chen primes: primes p such that p + 2 is either a prime or a
 *          semiprime.
 * https://oeis.org/A109611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth Chen prime (prime p where p+2 is prime or semiprime).
 *            Computed by scanning integers and using is_prime and
 *            is_semiprime_uint64
 * Range:     u8 n <= 41, u16 n <= 2992, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   is_prime, is_semiprime_uint64, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_semiprime_uint64.h"
#include "../utils/push_seq.h"

static idx_t next_candidate = 2;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;
static seq_t *cache = NULL;

seq_t a(idx_t n) {
    /* Return the nth Chen prime. */
    if (n < 1) return 0;
    while (cache_size < n) {
        if (is_prime((uint64_t)next_candidate)) {
            uint64_t p2 = next_candidate + 2;
            if (is_prime(p2) || is_semiprime_uint64(p2)) {
                push_seq(&cache, &cache_cap, &cache_size, (seq_t)next_candidate);
            }
        }
        next_candidate++;
    }
    return cache[n - 1];
}
