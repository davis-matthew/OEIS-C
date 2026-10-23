/*
 * A103890  a(n) = prime(n)! / prime(n)# + 1.
 * https://oeis.org/A103890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)! / prime(n)# + 1 (product of all composite
 *            numbers ≤ p_n plus 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

static idx_t  max_n          = 0;            /* largest n we have prepared for        */
static uint8_t* is_prime     = NULL;         /* sieve array, 1 = prime, 0 = composite */
static uint64_t* prime_list  = NULL;         /* primes in ascending order             */
static idx_t   prime_cnt      = 0;           /* number of primes stored              */

/* Recompute sieve and prime list up to at least the nth prime */
static void recompute_sieve(idx_t n)
{
    /* Estimate an upper bound for the nth prime */
    uint64_t ub_u64 = estimate_prime_upper_bound(n);
    idx_t     ub     = (idx_t)ub_u64;   /* assumes ub <= UINT64_MAX   */

    /* Free any previous sieve / prime list */
    free(is_prime);
    free(prime_list);
    is_prime   = NULL;
    prime_list = NULL;

    /* Allocate sieve array */
    is_prime = (uint8_t*)malloc((size_t)(ub + 1));
    if (!is_prime) return;                /* out‑of‑memory: nothing to work with */

    for (idx_t i = 0; i <= ub; ++i)
        is_prime[i] = 1;                /* start as all prime candidates */
    if (ub >= 0) { is_prime[0] = 0; }
    if (ub >= 1) { is_prime[1] = 0; }

    /* Classic Eratosthenes sieve */
    for (idx_t i = 2; i * i <= ub; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= ub; j += i)
                is_prime[j] = 0;

    /* Count primes to allocate prime_list */
    idx_t cnt = 0;
    for (idx_t i = 2; i <= ub; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) { cnt = 0; }        /* on allocation failure leave empty list */

    idx_t pos = 0;
    for (idx_t i = 2; i <= ub; ++i)
        if (is_prime[i])
            prime_list[pos++] = i;

    /* Store state */
    prime_cnt = cnt;
    max_n     = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined, but guard against n = 0 */

    /* Ensure we have the nth prime */
    if (n > max_n)
        recompute_sieve(n);

    /* nth prime (1‑based) is stored at index n‑1 */
    uint64_t p = prime_list[n - 1];

    /* Product of all composite numbers ≤ p */
    seq_t prod = (seq_t)1;
    for (uint64_t i = 4; i <= p; ++i)
        if (!is_prime[(idx_t)i])
            prod = prod * (seq_t)i;

    return prod + (seq_t)1;
}
