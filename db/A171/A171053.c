/*
 * A171053  In the sequence of prime numbers, replace all the '9' digits with
 *          '4' and vice versa.
 * https://oeis.org/A171053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by taking the n‑th prime and swapping digits 9 and 4. */
#include <stdlib.h>
#include <stdint.h>

/* --- Prime cache ------------------------------------------------------- */
static uint64_t *prime_cache  = NULL;   /* array of primes */
static size_t prime_count  = 0;         /* number of primes stored */
static size_t prime_cap    = 0;         /* capacity of prime_cache */
static uint64_t next_candidate = 2;     /* next number to test for primality */

/* Ensure at least n primes are stored in prime_cache. */
static void ensure_primes(idx_t n)
{
    /* Grow the cache if needed */
    while (prime_count < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Test cand for primality */
        int is_prime = 1;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p > cand / p)          /* p*p would overflow or > cand */
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            /* Expand the array if necessary */
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);          /* out of memory */
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_count++] = cand;

            /* Prepare next candidate */
            next_candidate = cand + 1;
            if ((next_candidate & 1) == 0) next_candidate++;   /* skip evens except 2 */
        } else {
            /* Not prime; just advance candidate */
            ++next_candidate;
            if ((next_candidate & 1) == 0) next_candidate++;   /* skip evens */
        }
    }
}

seq_t a(idx_t n)
{
    /* Invalid indices are irrelevant; return 0 for completeness. */
    if (n < 1) return 0;

    /* Make sure we have the n‑th prime. */
    ensure_primes(n);

    /* Retrieve the prime. */
    uint64_t p = prime_cache[n - 1];

    /* Perform the digit substitution: 4 ↔ 9. */
    seq_t result = 0;
    seq_t power  = 1;

    while (p > 0) {
        seq_t d = (seq_t)(p % 10);
        if (d == 4)      d = 9;
        else if (d == 9) d = 4;

        result += d * power;
        power   *= 10;
        p       /= 10;
    }

    return result;
}
