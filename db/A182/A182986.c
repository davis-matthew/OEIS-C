/*
 * A182986  Zero together with the prime numbers (A000040).
 * https://oeis.org/A182986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; for n > 1, a(n) = the (n‑1)-th prime number
 * Range:     u8 n <= 55, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------
 * Prime sieve using simple incremental trial division.
 * Stores primes in a dynamically allocated array of uint64_t.
 * ------------------------------------------------------------- */
static uint64_t *prime_list     = NULL;   /* array of primes */
static size_t   prime_count    = 0;      /* number of primes stored   */
static size_t   prime_cap      = 0;      /* current capacity         */
static uint64_t next_candidate = 2;      /* next integer to test     */

/* Ensure that we have at least k primes (k >= 1). */
static void
ensure_primes_at(size_t k)
{
    if (prime_count >= k)
        return;

    while (prime_count < k) {
        bool is_prime = true;
        /* Test divisibility by known primes up to sqrt(next_candidate). */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Grow the storage if needed. */
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_cap * sizeof(uint64_t));
                if (!tmp)
                    abort();          /* allocation failure */
                prime_list = tmp;
                prime_cap   = new_cap;
            }
            prime_list[prime_count++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* -------------------------------------------------------------
 * Entry point for OEIS sequence A182986.
 * ------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;       /* the honorary prime 0 */

    /* We need the (n‑1)-th prime. */
    size_t k = (size_t)(n - 1);
    ensure_primes_at(k);
    return (seq_t)prime_list[k - 1];
}
