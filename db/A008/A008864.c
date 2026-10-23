/*
 * A008864  a(n) = prime(n) + 1.
 * https://oeis.org/A008864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + 1
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *prime_cache   = NULL;     /* stored primes */
static size_t   prime_count   = 0;         /* number of primes stored */
static size_t   prime_cap     = 0;         /* allocated capacity */
static uint64_t next_candidate = 2;        /* next number to test for primality */

/* Simple deterministic primality test, sufficient for p < 2^64. */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

/* Ensure that at least n primes are present in prime_cache. */
static void ensure_n_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    uint64_t p = next_candidate;
    while (prime_count < (size_t)n) {
        if (is_prime_candidate(p)) {
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                if (!tmp) {
                    /* Out‑of‑memory: abort program. */
                    abort();
                }
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_count++] = p;
        }
        ++p;
    }
    next_candidate = p;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined below offset */

    ensure_n_primes(n);

    seq_t prime = (seq_t)prime_cache[(size_t)(n - 1)];
    return prime + (seq_t)1;
}
