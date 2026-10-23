/*
 * A116568  Difference between n and the absolute value of the difference
 *          between number of nonprimes not exceeding n and number of primes
 *          not exceeding n.
 * https://oeis.org/A116568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - | n - 2*π(n) | * where π(n) is the number of primes ≤
 *            n
 * Range:     u8 n <= 272, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a growing binary‑sieve of Eratosthenes
 * on demand.  It works with any seq_t that is an unsigned integer
 * type (possibly an arbitrary‑precision class) and any idx_t that
 * is an unsigned 64‑bit index. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of the exported function */
seq_t a(idx_t n);

/* private state: a minimal sieve that can grow */
static idx_t max_sieve = 0;      /* highest index for which the sieve is valid */
static uint8_t *is_prime = NULL; /* 1 = prime, 0 = composite   */
static seq_t *pi_prefix = NULL;  /* pi_prefix[i] = π(i)         */

/* enlarge the sieve to cover at least n */
static void ensure_sieve(idx_t n)
{
    if (n <= max_sieve)               /* already large enough */
        return;

    idx_t new_size = n;

    /* reallocate arrays – C++ needs an explicit cast */
    is_prime  = (uint8_t *)realloc(is_prime,  (new_size + 1) * sizeof(uint8_t));
    pi_prefix = (seq_t  *)realloc(pi_prefix, (new_size + 1) * sizeof(seq_t));

    /* initialise the whole array to 1 (prime), then correct 0/1 */
    for (idx_t i = 0; i <= new_size; ++i)
        is_prime[i] = 1;
    if (new_size >= 0) is_prime[0] = 0;
    if (new_size >= 1) is_prime[1] = 0;

    /* classic Eratosthenes sieve up to new_size */
    for (idx_t p = 2; p * p <= new_size; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= new_size; j += p)
                is_prime[j] = 0;
        }
    }

    /* build prefix sums for π(n) */
    pi_prefix[0] = 0;
    for (idx_t i = 1; i <= new_size; ++i)
        pi_prefix[i] = pi_prefix[i - 1] + (seq_t)is_prime[i];

    max_sieve = new_size;
}

/* the exported routine */
seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined for 0, return 0 safely */
        return (seq_t)0;

    ensure_sieve(n);                /* guarantees that π(n) is known */

    seq_t nn   = (seq_t)n;
    seq_t pi_n = pi_prefix[n];

    /* compute signed difference: n - 2*π(n) */
    int64_t signed_diff = (int64_t)nn - 2 * (int64_t)pi_n;
    seq_t absdiff = signed_diff >= 0 ? (seq_t)signed_diff
                                     : (seq_t)(-signed_diff);

    return nn - absdiff;          /* = n - |n - 2*π(n)| */
}
