/*
 * A135066  Primes p such that p^3 is a palindrome.
 * https://oeis.org/A135066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = prime p such that p^3 is a palindrome (in base 10)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   append_candidate, is_palindrome, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/is_palindrome.h"
#include "../utils/append_candidate.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static variable to generate next prime candidate */
    static seq_t next_candidate = 2;

    /* Expand cache if needed */
    if (cache_len < (size_t)n) {
        while (cache_len < (size_t)n) {
            seq_t cand;
            /* Produce the next candidate number (skipping even numbers after 2) */
            if (next_candidate == 2) {
                cand = 2;
                next_candidate = 3;
            } else {
                cand = next_candidate;
                next_candidate += 2;   /* skip even numbers */
            }

            /* Test for primality */
            if (!is_prime_big(cand))
                continue;

            /* Compute cand^3 using big integer multiplication to avoid overflow */
            seq_t cube = seq_mul(cand, seq_mul(cand, cand));
            /* Check if the cube is a palindrome in base 10 */
            if (is_palindrome(cube, 10))
                append_candidate(&cache, &cache_len, &cache_cap, cand);
        }
    }
    return cache[n - 1];
}
