/*
 * A117668  a(n) is the n-th prime number if n is prime; a(n) is the n-th
 *          nonprime number otherwise.
 * https://oeis.org/A117668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = The n-th prime if n is prime; otherwise the n-th
 *            nonprime. 1 is the first nonprime, 2 is the first prime, etc
 * Range:     u8 n <= 58, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"  /* provides is_prime(idx_t) */

/* Forward declarations of internal helpers */
static void ensure_lists(idx_t n);

/* Static storage for primes and nonprimes lists */
static seq_t *primes = NULL;          /* 1‑indexed */
static seq_t *nonprimes = NULL;       /* 1‑indexed */
static size_t prime_count = 0;
static size_t nonprime_count = 0;
static idx_t sieve_limit = 0;

/* -------------------------------------------------------------------------- */
/* Recompute primes and nonprimes up to a given limit.                         */
/* -------------------------------------------------------------------------- */
static void recompute_sieve(idx_t limit)
{
    /* Allocate a scratch array for the sieve */
    unsigned char *isprime = (unsigned char *)malloc((size_t)(limit + 1));
    if (!isprime) { /* minimal error handling; assume enough memory */ }
    for (idx_t i = 0; i <= limit; ++i) isprime[i] = 1;
    isprime[0] = isprime[1] = 0;

    /* Simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (isprime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                isprime[j] = 0;
        }
    }

    /* Count primes and nonprimes */
    size_t p_cnt = 0;
    size_t np_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (isprime[i]) ++p_cnt;
    for (idx_t i = 1; i <= limit; ++i)
        if (!isprime[i]) ++np_cnt;

    /* Allocate new lists */
    seq_t *new_primes = (seq_t *)malloc((p_cnt + 1) * sizeof(seq_t));
    seq_t *new_nonprimes = (seq_t *)malloc((np_cnt + 1) * sizeof(seq_t));
    if (!new_primes || !new_nonprimes) {
        /* minimal error handling */
    }

    /* Fill the lists (1‑indexed) */
    size_t p_idx = 1;
    for (idx_t i = 2; i <= limit; ++i) if (isprime[i]) new_primes[p_idx++] = (seq_t)i;
    size_t np_idx = 1;
    for (idx_t i = 1; i <= limit; ++i) if (!isprime[i]) new_nonprimes[np_idx++] = (seq_t)i;

    /* Free old memory if present */
    free(primes);
    free(nonprimes);

    /* Commit new data */
    primes = new_primes;
    nonprimes = new_nonprimes;
    prime_count = p_cnt;
    nonprime_count = np_cnt;
    sieve_limit = limit;

    free(isprime);
}

/* -------------------------------------------------------------------------- */
/* Ensure that the prime/nonprime lists contain at least n entries.            */
/* If not, extend the sieve until it does.                                    */
/* -------------------------------------------------------------------------- */
static void ensure_lists(idx_t n)
{
    if (prime_count >= n && nonprime_count >= n) return;

    /* Start with a heuristic limit */
    idx_t new_limit = sieve_limit ? sieve_limit * 2 : (idx_t)(n * 15 + 100);
    if (new_limit < 100) new_limit = 100;

    /* Extend until both lists are long enough */
    while (prime_count < n || nonprime_count < n) {
        recompute_sieve(new_limit);
        if (prime_count < n || nonprime_count < n)
            new_limit *= 2;
    }
}

/* -------------------------------------------------------------------------- */
/* Main sequence function: returns a(n).                                       */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n == 0; guard used only for safety */

    bool n_is_prime = is_prime(n);

    /* Make sure we have enough data */
    ensure_lists(n);

    if (n_is_prime)
        return primes[(size_t)n];
    else
        return nonprimes[(size_t)n];
}
