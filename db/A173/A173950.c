/*
 * A173950  a(n) = 1 if 6 divides (prime(n) + 1), a(n) = -1 if 6 divides
 *          (prime(n) - 1), a(n) = 0 otherwise.
 * https://oeis.org/A173950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if 6 divides (prime(n)+1), a(n) = -1 if 6 divides
 *            (prime(n)-1), a(n) = 0 otherwise
 * Range:     i8 n <= 31, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Static storage for the first primes (enough for n up to ~11000). */
static uint32_t primes[11000];
static size_t prime_count = 0;
static uint32_t next_candidate = 3;

/* Return the n-th prime (1‑based).  The function builds the prime list on
   first request and extends it if necessary.  The implementation uses
   trial division by the primes already known. */
static uint32_t get_nth_prime(idx_t n)
{
    if (n == 0) return 0;                     /* defensive guard */
    if (prime_count == 0) {                    /* insert the first prime */
        primes[prime_count++] = 2;
        next_candidate = 3;
    }

    while (prime_count < (size_t)n) {
        uint32_t c = next_candidate;
        bool isprime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > c) break;
            if (c % p == 0) { isprime = false; break; }
        }
        if (isprime) primes[prime_count++] = c;
        next_candidate += 2;                   /* test only odd numbers */
    }
    return primes[(size_t)n - 1];
}

/* The sequence function required by the evaluator.  It computes the
   nth prime and applies the modulo‑6 test to decide the value. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* offset handling */
    seq_t p = (seq_t)get_nth_prime(n);

    if (((p + (seq_t)1) % 6) == 0) return (seq_t)1;
    if (((p - (seq_t)1) % 6) == 0) return (seq_t)-1;
    return (seq_t)0;
}
