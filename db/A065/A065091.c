/*
 * A065091  Odd primes.
 * https://oeis.org/A065091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the (n+1)-th prime (primes[0] = 2)
 * Range:     u8 n <= 53, u16 n <= 6541, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state: prime cache */
    static uint64_t *prime_cache = NULL;
    static size_t prime_cnt   = 0;   /* number of primes stored   */
    static size_t prime_cap   = 0;   /* capacity of prime_cache   */
    static uint64_t next_candidate = 3;   /* next odd candidate to test */

    /* On first call, initialise cache with the first prime */
    if (prime_cap == 0) {
        prime_cap = 32;                            /* initial capacity   */
        prime_cache = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) return (seq_t)0;          /* OOM – unlikely       */
        prime_cache[0] = 2;                       /* the first prime */
        prime_cnt    = 1;
    }

    /* We need the prime at 0‑based index equal to n (n≥1 → 2nd prime is 3) */
    size_t target_index = (size_t)n;

    /* Generate primes until we have the desired one */
    while (prime_cnt <= target_index) {
        uint64_t p = next_candidate;
        bool is_prime = true;

        /* Trial division by previously found primes */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_cache[i];
            if (pr * pr > p) break;
            if (p % pr == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Ensure capacity */
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) return (seq_t)0;          /* OOM – should not happen */
                prime_cache = tmp;
            }
            prime_cache[prime_cnt++] = p;           /* store new prime */
        }

        /* Prepare next odd candidate */
        next_candidate += (next_candidate == 3) ? 2 : 2;
    }

    return (seq_t)prime_cache[target_index];
}
