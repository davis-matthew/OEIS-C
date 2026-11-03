/*
 * A095375  Total number of 1's in the binary expansions of the first n
 *          primes: summatory A014499.
 * https://oeis.org/A095375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 7822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9839 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A095375:
   a(n) = total number of 1's in the binary expansions of the first n primes.
   The function generates primes up to the n‑th and sums the popcounts of
   each prime.  The result is returned as type seq_t.  No external
   libraries or fixed lookup tables are used. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether a candidate number is prime using the already
   generated primes.  Only trial division up to sqrt(candidate) is
   performed.  The function is inline for speed and keeps its own static
   list of primes. */
static bool prime_is_prime(uint32_t candidate, uint32_t *primes, size_t prime_cnt)
{
    if (candidate < 2) return false;
    if (candidate == 2) return true;
    if ((candidate & 1) == 0) return false;          /* even numbers are composite */

    /* trial division by known primes only up to sqrt(candidate) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* a(1) corresponds to the first prime 2  (sum of popcounts 1) */
    if (n == 0) return (seq_t)0;          /* offset is 1 – undefined below 1 */

    /* Static storage for generated primes.  The array grows only as
       needed and is reused across calls. */
    static uint32_t *primes     = NULL;
    static size_t   prime_cnt   = 0;      /* number of primes stored */
    static size_t   prime_cap   = 0;      /* allocated capacity      */

    /* Ensure we have at least n primes.  If not, generate them. */
    if (prime_cnt < (size_t)n) {
        /* Grow capacity until it can hold n primes */
        size_t new_cap = prime_cap ? prime_cap : 4;
        while (new_cap < (size_t)n) new_cap <<= 1;

        if (new_cap > prime_cap) {
            uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
            if (!tmp) return (seq_t)0;  /* allocation failed */
            primes = tmp;
            prime_cap = new_cap;
        }

        /* Start generating primes from the next candidate after the
           last stored prime, or 2 if we have none. */
        uint32_t candidate = (prime_cnt == 0) ? 2
                                               : primes[prime_cnt - 1] + 1;
        if (candidate > 2 && (candidate & 1) == 0) candidate++;  /* keep odd */

        while (prime_cnt < (size_t)n) {
            if (prime_is_prime(candidate, primes, prime_cnt)) {
                primes[prime_cnt++] = candidate;
            }
            ++candidate;
            if ((candidate & 1) == 0) ++candidate; /* skip even numbers */
        }
    }

    /* Compute the cumulative popcount sum for the first n primes.  The
       sum is always small enough to fit in seq_t after each addition. */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < (size_t)n; ++i) {
        sum += (seq_t)__builtin_popcount(primes[i]);
    }
    return sum;
}
