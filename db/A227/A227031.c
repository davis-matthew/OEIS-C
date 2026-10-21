/*
 * A227031  Odd primes such that the previous prime is not the larger part of
 *          a twin prime pair.
 * https://oeis.org/A227031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 5682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227031: odd primes p such that prevprime(p)-2 is not prime.
   Computed by iterating over primes and checking the condition. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

#define TERMS_CAP 200000  /* enough to hold >10 000 terms */

/* Memoised terms of the sequence. */
static seq_t terms[TERMS_CAP];
static size_t terms_count = 0;

/* Current state of prime generation. */
static uint64_t prime_candidate = 1;  /* next integer to test */
static uint64_t last_prime      = 2;   /* previous prime found, starts at 2 */

/* Advance to the next prime and update the static state. */
static void advance_prime(void)
{
    while (true) {
        prime_candidate++;
        if (prime_candidate > 3 && !(prime_candidate & 1U))
            continue;  /* skip even numbers > 2 */

        if (!is_prime(prime_candidate))
            continue;  /* not a prime */

        uint64_t p = prime_candidate;   /* current prime */
        uint64_t q = last_prime;        /* previous prime */

        /* Condition: q-2 is NOT prime. */
        bool ok = true;
        if (q >= 2) {
            uint64_t qm2 = q - 2;
            if (is_prime(qm2))
                ok = false;
        }

        /* Store odd prime that satisfies the condition. */
        if (ok && (p & 1U)) {
            if (terms_count < TERMS_CAP)
                terms[terms_count++] = (seq_t)p;
        }

        last_prime = p;
        break;  /* one prime processed */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* undefined for n = 0 */

    while (terms_count < (size_t)n)
        advance_prime();

    return terms[n - 1];
}
