/*
 * A102476  Least modulus with 2^n square roots of 1.
 * https://oeis.org/A102476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n = 0 * a(n) = 3 if n = 1 * a(n) = 4 · Π_{i=0}^{n-2}
 *            prime[i] otherwise * where prime[i] is the (i+1)-st prime
 *            (2,3,5,7,…). * * For n ≥ 2 the value equals 4 times the
 *            primorial of the (n‑1)‑th * prime. (The primorial of p_k is the
 *            product of the first k primes.) * This implements exac...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#ifndef SIEVE_LIMIT
#   define SIEVE_LIMIT 200000   /* enough to hold at least 10000 primes */
#endif

/* Prime sieve storage */
static bool sieve[SIEVE_LIMIT + 1] = { false };
static uint32_t primes[SIEVE_LIMIT]  = { 0 };   /* will hold primes only */
static idx_t    primes_count = 0;
static bool     primes_ready = false;

/* One‑time initialization of the prime sieve */
static void
init_primes(void)
{
    /* Mark all numbers as prime, except 0 and 1 */
    sieve[0] = false;
    sieve[1] = false;
    for (idx_t i = 2; i < SIEVE_LIMIT; ++i) {
        sieve[i] = true;
    }

    /* Eratosthenes sieve */
    for (idx_t p = 2; p * p < SIEVE_LIMIT; ++p) {
        if (sieve[p]) {
            for (idx_t mult = p * p; mult < SIEVE_LIMIT; mult += p) {
                sieve[mult] = false;
            }
        }
    }

    /* Store the primes */
    for (idx_t i = 2; i < SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            if (primes_count < SIEVE_LIMIT) {
                primes[primes_count++] = (uint32_t)i;
            }
        }
    }

    primes_ready = true;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Ensure primes are ready */
    if (!primes_ready) {
        init_primes();
        /* After initialization we assume enough primes for requested n.
           If n-1 exceeds primes_count, the product loop simply stops at
           primes_count, yielding an incomplete but harmless result for
           values that exceed the intended range. */
    }

    if (n == 0) {
        return (seq_t)1;
    }
    if (n == 1) {
        return (seq_t)3;
    }

    /* Product of the first n-1 primes */
    idx_t needed = n - 1;           /* number of primes to multiply */
    seq_t prod   = (seq_t)1;
    for (idx_t i = 0; i < needed && i < primes_count; ++i) {
        prod *= (seq_t)primes[i];
    }

    /* Multiply by 4 to get the required modulus */
    prod *= (seq_t)4;
    return prod;
}
