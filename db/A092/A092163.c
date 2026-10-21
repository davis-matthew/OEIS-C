/*
 * A092163  a(n) = prime(2n) + prime(2n+1).
 * https://oeis.org/A092163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n) + prime(2*n+1)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Use a bound that contains at least the 20001‑st prime,
   which guarantees 2*n+1 primes for all n ≤ 10000. */
#define PRIME_LIMIT 300000

/* Static storage for the primes found by the sieve. */
static idx_t primes[PRIME_LIMIT];
static idx_t prime_count = 0;
static bool primes_initialised = false;

/* Sieve of Eratosthenes that fills `primes` and `prime_count`. */
static void init_primes(void)
{
    static bool is_prime[PRIME_LIMIT + 1];
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        is_prime[i] = true;

    for (idx_t i = 2; (idx_t)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    prime_count = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            primes[prime_count++] = i;
        }
    }
    primes_initialised = true;
}

/* Return a(n) = prime(2n) + prime(2n+1).  The OEIS offset is 1,
   so a(1) uses prime(2) and prime(3). */
seq_t a(idx_t n)
{
    if (!primes_initialised)
        init_primes();

    /* n must be ≥ 1; for n < 1 the behaviour is undefined. */
    if (n < 1)
        return 0;

    /* Ensure we have enough primes for indices 2*n and 2*n+1. */
    if ((idx_t)(2 * n + 1) > prime_count)
        return 0;  /* should not happen for the tested ranges */

    /* 2*n --> index 2*n-1 in the zero‑based array `primes`; 2*n+1 --> index 2*n. */
    seq_t p1 = (seq_t)primes[2 * n - 1];
    seq_t p2 = (seq_t)primes[2 * n];
    return p1 + p2;
}
