/*
 * A023530  Sum of exponents in prime-power factorization of
 *          prime(n)*prime(n-1) + 1; a(1) = 1.
 * https://oeis.org/A023530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of exponents in the prime‑power factorization * of
 *            prime(n) * prime(n-1) + 1; a(1) = 1. * * This implementation
 *            precomputes primes as needed and then factors * the product by
 *            trial division using the cached primes. The factor * counting is
 *            performed on a 64‑bit unsigned value, which is...
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 7088, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of primes (1‑based).  prime_cache[0] = 2. */
static uint32_t *prime_cache = NULL;
static size_t prime_cnt   = 0;   /* number of primes stored           */
static size_t prime_cap   = 0;   /* allocated size of prime_cache     */

/* Ensure prime_cache contains at least (index+1) primes. */
static void ensure_primes(idx_t index)
{
    while (prime_cnt <= (size_t)index) {
        /* Find next prime after prime_cache[prime_cnt-1] (or 2 if first). */
        uint32_t candidate = (prime_cnt == 0) ? 2 : prime_cache[prime_cnt - 1] + 1;
        while (1) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cnt; ++i) {
                if ((seq_t)prime_cache[i] * prime_cache[i] > candidate)
                    break;
                if (candidate % prime_cache[i] == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                /* Grow cache if needed. */
                if (prime_cnt == prime_cap) {
                    size_t new_cap = prime_cap == 0 ? 8 : prime_cap * 2;
                    uint32_t *tmp = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
                    if (!tmp) abort();          /* allocation failure */
                    prime_cache = tmp;
                    prime_cap   = new_cap;
                }
                prime_cache[prime_cnt++] = candidate;
                break;
            }
            ++candidate;
        }
    }
}

/* Count total number of prime factors (with multiplicity) of m
 * using the cached primes.  Assumes m fits in uint64_t. */
static seq_t omega_from_primes(seq_t m)
{
    uint64_t m64 = (uint64_t)m;
    seq_t count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p * p > m64) break;
        while (m64 % p == 0) {
            m64 /= p;
            ++count;
        }
    }
    if (m64 > 1) ++count;   /* remaining prime factor */
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    if (n == 1) return 1;         /* a(1) is defined to be 1 */

    /* Ensure primes p(n) and p(n-1) are available. */
    ensure_primes(n);                 /* prime_cnt > n */
    uint32_t pn  = prime_cache[n - 1]; /* p(n)  */
    uint32_t pn1 = prime_cache[n - 2]; /* p(n-1) */

    seq_t product = (seq_t)pn * pn1 + 1;  /* prime(n)*prime(n-1)+1 */
    return omega_from_primes(product);
}
