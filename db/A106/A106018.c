/*
 * A106018  Primes p such that 22*p +- 3 are primes.
 * https://oeis.org/A106018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 267, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search.  The sequence consists of primes p such that  
   both 22·p – 3 and 22·p + 3 are prime.  Offset 1.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; values for n ≤ 0 are irrelevant. */
    if (n == 0) return 0;

    /* Static cache of found terms.  20 000 slots are more than enough
       for n ≤ 10 000 (the 10 000th term is below 2 · 10⁶). */
    static seq_t cache[20000];
    static size_t cache_sz = 0;

    /* Candidate for the next prime to test. */
    static uint64_t cand = 2;

    while (cache_sz < n)
    {
        /* Find the next prime candidate. */
        while (!is_prime(cand))
            ++cand;

        /* Check the two derived values. */
        uint64_t v1 = 22ULL * cand - 3ULL;
        uint64_t v2 = 22ULL * cand + 3ULL;

        if (is_prime(v1) && is_prime(v2))
            cache[cache_sz++] = (seq_t)cand;

        /* Move on to the following integer. */
        ++cand;
    }

    return cache[n - 1];
}
