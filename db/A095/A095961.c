/*
 * A095961  If p(x) is the x-th prime, then the n-th set of 2 consecutive sexy
 *          prime pairs starts at p(a(n)).
 * https://oeis.org/A095961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of 1st prime in the nth occurrence of two
 *            consecutive sexy prime pairs
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Memoization cache for found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* State for generating primes incrementally. */
    static uint64_t last[4]      = {0};   /* recent prime values (oldest at 0, newest at 3) */
    static idx_t   lastIdx[4]    = {0};
    static size_t  lastSize     = 0;      /* how many primes stored (max 4) */
    static idx_t   prime_index  = 0;      /* count of primes seen so far */
    static uint64_t candidate   = 3;      /* next integer to test for primality */

    /* Return cached value if available. */
    if (cache_len >= n)
        return cache[n - 1];

    /* Generate primes until we have found n patterns. */
    while (cache_len < n) {
        uint64_t cur;          /* current prime */

        /* Special handling for the very first prime. */
        if (prime_index == 0) {
            cur = 2;
            prime_index = 1;
        } else {
            /* Find the next prime. */
            while (!is_prime(candidate))
                candidate++;
            cur = candidate;
            prime_index++;
            /* Advance candidate for the next call. */
            candidate++;
        }

        /* Keep a sliding window of the last four primes. */
        if (lastSize < 4) {
            last[lastSize] = cur;
            lastIdx[lastSize] = prime_index;
            lastSize++;
        } else {
            /* Shift left to discard the oldest prime. */
            for (int k = 0; k < 3; k++) {
                last[k]     = last[k + 1];
                lastIdx[k]  = lastIdx[k + 1];
            }
            last[3] = cur;
            lastIdx[3] = prime_index;

            /* Check for two consecutive sexy prime pairs. */
            if (last[0] + 6 == last[1] && last[2] + 6 == last[3]) {
                grow_seq(&cache, &cache_cap, cache_len + 1);
                cache[cache_len++] = (seq_t)lastIdx[0];
            }
        }
    }

    return cache[n - 1];
}
