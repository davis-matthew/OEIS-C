/*
 * A104120  a(n) = (prime(n + 1) - prime(n))/2 (mod 2).
 * https://oeis.org/A104120
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((prime(n+1) - prime(n))/2) mod 2, offset 2. The function
 *            computes the required primes on demand using a simple sieve of
 *            Eratosthenes. The estimate_prime_upper_bound helper gives an
 *            upper bound for the (n+1)-th prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

static uint64_t *prime_table = NULL;   /* 0‑based prime list */
static size_t   prime_count = 0;       /* number of primes stored */

/* Expand the prime table so that it contains at least 'need' primes.
   The estimate_prime_upper_bound function returns an upper bound for
   the 'need'-th prime. */
static void extend_primes(size_t need)
{
    if (prime_count >= need) return;

    /* determine an upper bound for the required prime */
    size_t limit = estimate_prime_upper_bound(need);
    /* safety margin */
    limit += 100;
    if (limit < 100) limit = 100;

    /* sieve up to 'limit' */
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) exit(EXIT_FAILURE);       /* out of memory */
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;

    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* count primes and grow storage if needed */
    size_t new_count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++new_count;

    uint64_t *new_table = (uint64_t *)realloc(prime_table,
                                            new_count * sizeof(uint64_t));
    if (!new_table && new_count > 0) exit(EXIT_FAILURE);
    prime_table = new_table;

    /* fill table */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_table[idx++] = (uint64_t)i;

    prime_count = new_count;
    free(is_prime);
}

seq_t a(idx_t n)
{
    /* offset 2: valid for n >= 2, others return 0 */
    if (n < 2) return 0;

    /* need primes indexed n (prime[n+1]) and n-1 (prime[n]) */
    size_t need = (size_t)n + 1;
    extend_primes(need);

    uint64_t p_next = prime_table[need - 1];          /* prime(n+1) */
    uint64_t p_prev = prime_table[need - 2];          /* prime(n)   */
    uint64_t gap  = p_next - p_prev;
    uint64_t half = gap >> 1;                         /* (gap/2) truncated */

    return (seq_t)(half & 1ULL);                     /* parity of half */
}
