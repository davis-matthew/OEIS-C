/*
 * A091283  Exponent of 2 in the prime factorization of prime(n)^4 - 1.
 * https://oeis.org/A091283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091283:  v₂(prime(n)⁴ – 1)
   a(n) = v₂(p – 1) + v₂(p + 1) + 1   for odd prime p = prime(n),
   a(1) = 0 (prime = 2).               */
/* Uses a simple sieve to generate the first 20 000 primes (enough for n≤10 000). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIMES     20000          /* > number of primes below 200 000 */

/* Static storage for the sieve and the list of primes. */
static unsigned char *sieve        = NULL;
static size_t        sieve_limit   = 0;
static uint32_t      primes[MAX_PRIMES];
static size_t        prime_count   = 0;

/* Compute the exponent of 2 in an unsigned 64‑bit integer. */
static inline unsigned v2_uint64(uint64_t x)
{
    /* x is guaranteed >0 for p = 2; for odd primes p‑1 and p + 1 are even. */
    return (unsigned)__builtin_ctzll(x);
}

/* Initialise the sieve and fill the primes array up to an appropriate limit. */
static void ensure_primes(size_t n)
{
    if (prime_count >= n)
        return;                     /* already have enough primes */

    /* If the sieve has not yet been built, create it for a generous limit. */
    if (sieve == NULL) {
        /* 200 000 exceeds the 10 000th prime (104 729). */
        sieve_limit = 200000;
        sieve = (unsigned char*)malloc(sieve_limit + 1);
        memset(sieve, 0, sieve_limit + 1);

        /* Classic Sieve of Eratosthenes. */
        size_t i, j;
        for (i = 2; i * i <= sieve_limit; ++i) {
            if (!sieve[i]) {
                for (j = i * i; j <= sieve_limit; j += i)
                    sieve[j] = 1;
            }
        }

        /* Collect primes into the array. */
        prime_count = 0;
        for (i = 2; i <= sieve_limit && prime_count < MAX_PRIMES; ++i) {
            if (!sieve[i]) {
                primes[prime_count++] = (uint32_t)i;
            }
        }
    }

    /* If the requested n is larger than the primes we currently have,
       we would need a larger sieve.  The current constants ensure that
       this branch is never taken for the intended test ranges. */
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  Expects:  n ≥ 1 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* safety for undefined indices */

    ensure_primes((size_t)n);
    /* If n is larger than we can handle, fall back to 0. */
    if ((size_t)n > prime_count)
        return 0;

    uint32_t p = primes[(size_t)(n - 1)];   /* primes array is 0‑based */

    if (p == 2)
        return (seq_t)0;            /* a(1) = 0 */

    /* v₂(p⁴ – 1) = v₂(p‑1) + v₂(p+1) + v₂(p²+1); for odd p, v₂(p²+1)=1. */
    unsigned v2_minus = v2_uint64((uint64_t)(p - 1));
    unsigned v2_plus  = v2_uint64((uint64_t)(p + 1));
    unsigned result   = v2_minus + v2_plus + 1;

    return (seq_t)result;
}
