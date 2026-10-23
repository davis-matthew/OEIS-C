/*
 * A161753  Squares of nonprime numbers A141468.
 * https://oeis.org/A161753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [nth nonprime integer]^2
 * Range:     u8 n <= 10, u16 n <= 202, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool *sieve = NULL;
static idx_t *prime_prefix = NULL;
static idx_t sieve_limit = 0;

/* Build a sieve of Eratosthenes up to "limit" and a prefix count of primes. */
static void rebuild_sieve(idx_t limit)
{
    if (sieve) {
        free(sieve);
    }
    if (prime_prefix) {
        free(prime_prefix);
    }
    sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!sieve) return;                 /* allocation failure – highly unlikely for the limits used */
    for (idx_t i = 0; i <= limit; ++i) {
        sieve[i] = true;
    }
    if (limit >= 0) sieve[0] = false;
    if (limit >= 1) sieve[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (sieve[p]) {
            for (idx_t k = p * p; k <= limit; k += p) {
                sieve[k] = false;
            }
        }
    }

    prime_prefix = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!prime_prefix) return;
    prime_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        prime_prefix[i] = prime_prefix[i - 1] + (sieve[i] ? 1 : 0);
    }
    sieve_limit = limit;
}

/* Count the number of nonprime integers in [0, x]. */
static idx_t count_nonprime(idx_t x)
{
    return x + 1 - prime_prefix[x];
}

/* Return the nth term of OEIS A141468 (squares of nonprime numbers). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* safety for indices below the offset */

    /* Ensure the sieve is large enough to contain at least n nonprimes. */
    idx_t target_limit = (n < 3) ? 3 : n * 2;
    if (!sieve || sieve_limit < target_limit) {
        rebuild_sieve(target_limit);
    }
    while (count_nonprime(sieve_limit) < n) {
        idx_t new_limit = sieve_limit * 2;
        rebuild_sieve(new_limit);
    }

    /* Binary search for the nth nonprime integer. */
    idx_t low = 0, high = sieve_limit;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (count_nonprime(mid) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    idx_t val = low;
    seq_t result = (seq_t)val * (seq_t)val;
    return result;
}
