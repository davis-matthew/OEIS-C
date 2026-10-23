/*
 * A139101  Numbers that show the distribution of prime numbers up to the n-th
 *          prime minus 1, using "0" for primes and "1" for nonprime numbers.
 * https://oeis.org/A139101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   _prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation of digits d_i (i = 1 .. p_n-1) where
 * d_i = 0 if i is prime and 1 otherwise; p_n denotes the n‑th prime.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/_prime_limit_for_idx.h"

/* Cached sieve and prime list up to the largest n seen so far. */
static uint64_t *primes     = NULL;
static size_t   primes_count = 0;
static char    *sieve       = NULL;
static size_t   sieve_size   = 0;

/* Build or increase the sieve and prime list until at least the n‑th
 * prime is available.  The helper _prime_limit_for_idx supplies a rough
 * upper bound for the n‑th prime; if the bound is insufficient we
 * double it until enough primes are found. */
static void ensure_nth_prime(idx_t n)
{
    size_t limit = (size_t)_prime_limit_for_idx(n);
    if (limit == 0) limit = 3;   /* safety for small n */

    while (true) {
        if (sieve_size <= limit) {
            if (sieve) free(sieve);
            sieve = (char*)malloc((limit + 1) * sizeof(char));
            if (!sieve) exit(EXIT_FAILURE);

            for (size_t i = 0; i <= limit; ++i) sieve[i] = 1;
            sieve[0] = sieve[1] = 0;
            for (size_t p = 2; p * p <= limit; ++p) {
                if (sieve[p]) {
                    for (size_t j = p * p; j <= limit; j += p)
                        sieve[j] = 0;
                }
            }

            size_t cnt = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (sieve[i]) ++cnt;

            uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
            if (!new_primes) exit(EXIT_FAILURE);
            size_t idx = 0;
            for (size_t i = 2; i <= limit; ++i)
                if (sieve[i]) new_primes[idx++] = (uint64_t)i;

            if (primes) free(primes);
            primes = new_primes;
            primes_count = cnt;
            sieve_size   = limit + 1;
        }

        if (primes_count >= (size_t)n)
            break;

        limit *= 2;   /* enlarge the bound and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety: OEIS offset starts at 1 */

    ensure_nth_prime(n);

    uint64_t pn = primes[(size_t)(n - 1)];   /* nth prime (1‑based) */

    seq_t result = 0;
    for (uint64_t i = 1; i < pn; ++i) {
        unsigned char digit = (sieve[i] ? 0 : 1);   /* 0 for prime, 1 otherwise */
        result = result * (seq_t)10 + digit;
    }
    return result;
}
