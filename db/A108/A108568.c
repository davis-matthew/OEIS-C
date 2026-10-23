/*
 * A108568  a(n) = prime(n) + prime(n+1) - 2n - 1.
 * https://oeis.org/A108568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n) + prime(n+1) - 2*n - 1
 * Range:     u8 n <= 38, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *primes = NULL;   /* array of discovered primes (2,3,5,…)          */
static size_t   prime_count = 0;  /* number of primes currently stored           */
static size_t   primes_cap  = 0;  /* current capacity of primes array            */
static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Append a new prime to the array, expanding capacity if needed */
static void add_prime(uint64_t p) {
    if (prime_count == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* out of memory – terminate */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[prime_count++] = p;
}

/* Generate primes until we have at least 'needed' primes
   (needed is indexed from 1, so primes[0] = prime(1) = 2)      */
static void ensure_primes(size_t needed) {
    while (prime_count < needed) {
        uint64_t cand = next_candidate;

        /* Skip even numbers greater than 2 for efficiency */
        if (cand >= 3 && (cand & 1) == 0) {
            next_candidate = cand + 1;
            continue;
        }

        bool is_p = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            add_prime(cand);

        ++next_candidate;
    }
}

/* Public sequence function.  n is 1‑based.  Returns prime(n)+prime(n+1)-2n-1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n < 1 – guard guard */
    /* Need primes up to index n+1 (since prime(n+1) accessed) */
    ensure_primes((size_t)n + 1);

    seq_t pn   = (seq_t)primes[(size_t)n - 1];   /* prime(n)  */
    seq_t pn1  = (seq_t)primes[(size_t)n];       /* prime(n+1)*/
    seq_t sub  = (seq_t)2 * (seq_t)n + 1;        /* 2n+1      */
    return pn + pn1 - sub;
}
