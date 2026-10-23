/*
 * A155851  n is prime and is the sum of the first k primes for some k, start
 *          from 5.
 * https://oeis.org/A155851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime that equals the sum of the first k primes
 *            starting from the prime 5 (i.e., sums of consecutive primes
 *            5,7,11,13,⋯)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 15, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   append_candidate, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache  = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cumulative_sum = 0;
    static uint64_t current_prime = 5;   /* prime to add next */
    static uint64_t next_candidate = 7;  /* candidate for next prime generation */

    if (n < 1)
        return (seq_t)0;

    if (!initialized) {
        cache  = NULL;
        cache_len = 0;
        cache_cap = 0;
        cumulative_sum = 0;
        current_prime = 5;
        next_candidate = 7;
        initialized = true;
    }

    /* Generate terms until we have at least n entries cached. */
    while (cache_len < n) {
        /* Add the current prime to the running sum. */
        cumulative_sum = cumulative_sum + (seq_t)current_prime;

        /* If the resulting sum is prime, store it. */
        if (is_prime_big(cumulative_sum))
            append_candidate(&cache, &cache_len, &cache_cap, cumulative_sum);

        /* Find the next prime after current_prime. */
        uint64_t cand = current_prime + 2;   /* start from next odd */
        while (!is_prime((idx_t)cand))
            cand += 2;
        current_prime = cand;
    }

    return cache[n - 1];
}
