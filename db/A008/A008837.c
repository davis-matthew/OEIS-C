/*
 * A008837  a(n) = p*(p-1)/2 for p = prime(n).
 * https://oeis.org/A008837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(prime(n), 2) = prime(n)*(prime(n)-1)/2
 * Range:     u8 n <= 9, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Indexing: a(1) = 1, a(2) = 3, ...  (offset 1) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *prime_cache   = NULL;   /* dynamically grown array of primes */
static size_t    prime_count   = 0;       /* number of primes stored */
static size_t    prime_cap     = 0;       /* allocated capacity for prime_cache */
static uint64_t  next_candidate = 2;       /* next number to test for primality */

/* Incrementally fills prime_cache until it contains at least the nth prime (1‑based). */
static uint32_t get_prime_at(idx_t n)
{
    if (n == 0) return 0;
    while (prime_count < n) {
        bool is_prime = true;
        /* test divisibility by stored primes up to sqrt(next_candidate) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_cache[i];
            if ((uint64_t)p * p > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {          /* grow array */
                prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache,
                                                     prime_cap * sizeof(uint32_t));
                if (!tmp) exit(1);                    /* allocation failure */
                prime_cache = tmp;
            }
            prime_cache[prime_count++] = (uint32_t)next_candidate;
        }
        next_candidate += (next_candidate == 2) ? 1 : 2;  /* after 2 only odds */
    }
    return prime_cache[n - 1];
}

seq_t a(idx_t n)
{
    uint32_t p = get_prime_at(n);                      /* prime(n) */
    seq_t sp   = (seq_t)p;
    return sp * (sp - (seq_t)1) / (seq_t)2;
}
