/*
 * A076241  Remainder when 2nd order prime pp(n)=A006450(n) is divided by n.
 * https://oeis.org/A076241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pp(n) mod n, where pp(n) = p_{p_n} is the prime at index
 *            p_n (the nth prime of the primes). That is, pp(n) is the n‑th
 *            “second–order prime”
 * Range:     u8 n <= 16, u16 n <= 1223, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   estimate_np
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_np.h"

/* static state for the prime sieve */
static uint32_t *primes = NULL;          /* array of primes */
static size_t   prime_count = 0;        /* number of primes stored */
static size_t   sieve_limit  = 0;        /* highest number sieved */

/* Perform a fresh sieve up to new_limit (inclusive) */
static void sieve_up_to(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* allocate and init the sieve array */
    char *is_prime = (char *)malloc(new_limit + 1);
    if (!is_prime) return;                 /* out of memory – should not happen */

    memset(is_prime, 1, new_limit + 1);
    is_prime[0] = is_prime[1] = 0;

    size_t i, j;
    for (i = 2; i * i <= new_limit; ++i)
        if (is_prime[i])
            for (j = i * i; j <= new_limit; j += i)
                is_prime[j] = 0;

    /* count how many primes we found */
    size_t count = 0;
    for (i = 2; i <= new_limit; ++i)
        if (is_prime[i]) ++count;

    /* allocate prime list and fill it */
    uint32_t *new_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (i = 2; i <= new_limit; ++i)
        if (is_prime[i])
            new_primes[idx++] = (uint32_t)i;

    /* replace old prime table */
    free(primes);
    primes = new_primes;
    prime_count = count;
    sieve_limit  = new_limit;

    free(is_prime);
}

/* Ensure we have at least required_index primes stored (1‑based). */
static void ensure_primes_up_to_index(size_t required_index)
{
    if (prime_count >= required_index) return;

    size_t limit = estimate_np(required_index);
    if (limit < sieve_limit) limit = sieve_limit * 2 + 1; /* grow safely */
    sieve_up_to(limit);
}

/* the required OEIS entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, but guard */

    /* Step 1: obtain the nth prime p_n */
    ensure_primes_up_to_index((size_t)n);
    uint32_t p_n = primes[(size_t)n - 1];

    /* Step 2: obtain the prime at index p_n, i.e., pp(n) = p_{p_n} */
    ensure_primes_up_to_index((size_t)p_n);
    uint32_t pp_n = primes[(size_t)p_n - 1];

    /* Step 3: return pp(n) modulo n */
    return (seq_t)pp_n % (seq_t)n;
}
