/*
 * A138412  a(n) = p^5*(p-1) where p = prime(n).
 * https://oeis.org/A138412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p⁵·(p−1) where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 167, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */

/* Dynamically grow an array of primes. */
static uint32_t *primes   = NULL;   /* array of primes */
static size_t   primes_cnt  = 0;    /* number of primes stored */
static size_t   primes_cap  = 0;    /* capacity of the array */
static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Ensure that at least n primes have been generated. */
static void
ensure_primes(idx_t n)
{
    while (primes_cnt < n) {
        uint64_t cand = next_candidate;
        bool     is_prime = true;

        /* Trial division using the primes already found. */
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp) exit(1);   /* Allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)cand;
        }
        ++next_candidate;
    }
}

/* Return the nth term of OEIS A138412.  n >= 1. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;

    ensure_primes(n);          /* makes sure primes[n-1] exists */

    uint32_t   prime_u32 = primes[n - 1];
    uint64_t   prime_u64 = prime_u32;

    /* Compute p^5 * (p-1) using a 128‑bit intermediate to avoid overflow
       for the 64‑bit build; for arbitrary‑precision builds the cast
       will convert the value appropriately. */
    unsigned __int128 val = (unsigned __int128)prime_u64;
    val *= prime_u64;         /* p² */
    val *= prime_u64;         /* p³ */
    val *= prime_u64;         /* p⁴ */
    val *= prime_u64;         /* p⁵ */
    val *= (prime_u64 - 1);

    return (seq_t)val;
}
