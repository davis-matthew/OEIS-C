/*
 * A172367  Numbers k > 0 such that k+4 is a prime.
 * https://oeis.org/A172367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n+2)-4
 * Range:     u8 n <= 53, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for prime numbers generated so far. */
static uint64_t *primes = NULL;
static size_t primes_size = 0;   /* number of primes stored */
static size_t primes_cap = 0;    /* allocated capacity */
static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Ensure that at least 'target' primes have been generated.
   'target' is the index of the desired prime (1‑based).           */
static void ensure_primes_upto(idx_t target)
{
    while (primes_size < (size_t)target) {
        uint64_t cand = next_candidate;
        int is_prime = 1;
        for (size_t i = 0; i < primes_size; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            /* Grow the array if necessary */
            if (primes_size == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *new_ptr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!new_ptr) exit(1); /* out‑of‑memory, abort */
                primes = new_ptr;
                primes_cap = new_cap;
            }
            primes[primes_size++] = cand;   /* store the new prime */
        }
        next_candidate = cand + 1;           /* advance candidate */
    }
}

/* OEIS sequence A172367: numbers k > 0 such that k+4 is prime. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1, return 0 */
    idx_t target = n + 2;                /* we need the (n+2)-th prime */
    ensure_primes_upto(target);
    uint64_t prime_val = primes[target - 1]; /* 0‑based array */
    return (seq_t)prime_val - (seq_t)4;
}
