/*
 * A074395  A 7-way classification of the primes.
 * https://oeis.org/A074395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 7‑way classification of primes.
   For each prime p(n) (n ≥ 1) the function returns:
     - 6 if p(n) = 2
     - 0 for (p(n) ≡ 1 (mod 4), p(n+1) ≡ 3 (mod 4), twin primes)
     - 1 for (3, 1, twin)
     - 2 for (1, 3, non‑twin)
     - 3 for (3, 1, non‑twin)
     - 4 for (1, 1)
     - 5 for (3, 3)
   The classification uses the next prime p(n+1);  only 2 is handled
   specially.                                               */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* static storage for primes */
static uint64_t *primes = NULL;
static size_t primes_count = 0;
static uint64_t current_limit = 0;

/* Recompute all primes up to 'limit' */
static void recompute_primes(uint64_t limit)
{
    uint8_t *is_prime = (uint8_t*)malloc(limit + 1);
    if (!is_prime) return;

    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
    {
        if (is_prime[i])
        {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* estimate number of primes up to 'limit' */
    size_t capacity = (size_t)(limit / log((double)limit)) + 16;
    /* free old array if any */
    free(primes);
    primes = (uint64_t*)malloc(capacity * sizeof(uint64_t));
    if (!primes)
    {
        primes_count = 0;
        free(is_prime);
        return;
    }

    primes_count = 0;
    for (uint64_t i = 2; i <= limit; ++i)
    {
        if (is_prime[i])
            primes[primes_count++] = i;
    }

    current_limit = limit;
    free(is_prime);
}

/* Ensure we have at least (idx_needed + 1) primes (index starts at 0) */
static void ensure_primes(idx_t idx_needed)
{
    size_t needed = (size_t)idx_needed + 1;
    if (primes_count > idx_needed) return;    /* already enough */

    /* approximate upper bound on the (needed)th prime */
    uint64_t n = needed;
    double upper;
    if (n < 6)
        upper = 15.0;
    else
        upper = n * (log((double)n) + log(log((double)n)));

    uint64_t limit = (uint64_t)(upper + 10);
    recompute_primes(limit);
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; n>=1 expected */
    ensure_primes(n);            /* need primes[0..n] (n+1 primes) */

    uint64_t p = primes[n - 1];
    if (p == 2)
        return (seq_t)6;

    uint64_t p_next = primes[n];
    bool twin = (p_next - p == 2);
    uint8_t r1 = p & 3;
    uint8_t r2 = p_next & 3;
    seq_t val;

    if (r1 == 1 && r2 == 3 && twin)          val = (seq_t)0;
    else if (r1 == 3 && r2 == 1 && twin)     val = (seq_t)1;
    else if (r1 == 1 && r2 == 3 && !twin)    val = (seq_t)2;
    else if (r1 == 3 && r2 == 1 && !twin)    val = (seq_t)3;
    else if (r1 == 1 && r2 == 1)             val = (seq_t)4;
    else if (r1 == 3 && r2 == 3)             val = (seq_t)5;
    else                                     val = (seq_t)0;  /* safety */

    return val;
}
