/*
 * A151911  Abs(square of n-th prime minus cube of n-1).
 * https://oeis.org/A151911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |prime(n)^2 - (n-1)^3|, n ≥ 1
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamic array of primes (uint64_t) */
static uint64_t *primes = NULL;     /* allocated array of primes       */
static size_t    prime_cnt = 0;     /* number of primes stored         */
static size_t    prime_cap = 0;     /* current capacity of primes array */

/* Ensure that the prime array contains at least n primes.
   Generates new primes by simple trial division. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;                     /* already have enough primes */

    /* Initialise array on first call */
    if (prime_cnt == 0) {
        prime_cap = 8;               /* start with room for 8 primes */
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

    while (prime_cnt < n) {
        bool is_prime = true;

        /* Test divisibility by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)                 /* p^2 > candidate */
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            if (prime_cnt == prime_cap) {          /* grow the array */
                prime_cap *= 2;
                primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = candidate;       /* store new prime */
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based index) */
    ensure_primes(n);
    uint64_t p = primes[n - 1];                     /* safe: prime_cnt >= n */

    /* convert to seq_t for arithmetic */
    seq_t pn   = (seq_t)p;
    seq_t sq   = pn * pn;                           /* prime^2 */

    /* cube of (n-1) */
    seq_t m    = (seq_t)n - (seq_t)1;               /* m = n-1 */
    seq_t cube = m * m * m;                          /* (n-1)^3 */

    /* absolute difference */
    if (sq > cube)
        return sq - cube;
    else
        return cube - sq;
}
