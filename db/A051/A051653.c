/*
 * A051653  Primes p such that 2310*p + 1 is also prime.
 * https://oeis.org/A051653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find primes p such that 2310*p+1 is also prime
   (OEIS A051653). This implementation keeps a static cache
   of found terms and extends it as needed. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime64.h"   /* bool is_prime64(uint64_t n); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; ignore n=0 */

    /* Static cache: we preallocate space for up to 20000 terms.
       This exceeds the required 10000‑term usage in the tests. */
    static seq_t cache[20000];
    static size_t cache_cnt = 0;      /* number of terms currently cached */
    static uint64_t next_candidate = 2; /* next integer to test as prime */

    /* Extend the cache until we have enough terms. */
    while (cache_cnt < (size_t)n) {
        /* Skip even numbers after 2 for efficiency. */
        if (next_candidate > 2 && !(next_candidate & 1ULL)) {
            ++next_candidate;
            continue;
        }

        /* Check whether next_candidate is prime. */
        if (is_prime64(next_candidate)) {
            /* Compute 2310*p + 1. Use seq_t arithmetic to avoid overflow
               when seq_t is a big integer type. */
            seq_t cand_val = (seq_t)next_candidate * (seq_t)2310 + 1;

            /* Test whether 2310*p + 1 is also prime. */
            if (is_prime_big(cand_val)) {
                cache[cache_cnt++] = (seq_t)next_candidate;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
