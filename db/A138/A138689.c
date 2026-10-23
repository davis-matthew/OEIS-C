/*
 * A138689  Numbers of the form 26+p^2 (where p is a prime).
 * https://oeis.org/A138689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26 + p_n^2 where p_n is the nth prime
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for primes */
static uint64_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;
static uint64_t next_candidate = 3;

/* Return the nth prime (1‑based). Generates more primes on demand. */
static uint64_t get_nth_prime(idx_t n)
{
    /* Initialise cache on first use */
    if (prime_cnt == 0) {
        prime_cap = 256;
        prime_cache = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_cache) return 0;   /* failure, but may never occur */
        prime_cache[0] = 2;
        prime_cnt = 1;
        next_candidate = 3;
    }

    /* Generate primes until we have at least n of them */
    while (prime_cnt < (size_t)n) {
        uint64_t p = next_candidate;
        int is_prime = 1;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_cache[i];
            if (pr * pr > p) break;
            if (p % pr == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_cache = (uint64_t*)realloc(prime_cache, prime_cap * sizeof(uint64_t));
            }
            prime_cache[prime_cnt++] = p;
        }

        next_candidate += 2;   /* skip even numbers */
    }

    return prime_cache[n - 1];
}

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_nth_prime(n);
    return p * p + (seq_t)26;
}
