/*
 * A039738  a(n)=number of primes q<p having (p mod q)=4, where p=n-th prime.
 * https://oeis.org/A039738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes q<p_n such that p_n mod q == 4
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

/* Cache of the first primes needed so far */
static uint32_t *prime_ints = NULL;   /* array of first primes, 32‑bit */
static size_t   prime_cnt   = 0;      /* how many primes stored */

/* Cache of computed sequence values */
static seq_t   *a_vals  = NULL;  /* sequence values, 0‑based index */
static size_t   a_cap   = 0;      /* allocated capacity of a_vals   */
static size_t   a_used  = 0;      /* how many values have been computed */

/* Ensure we have room for at least 'needed' values in a_vals   */
static void
ensure_a_capacity(size_t needed)
{
    if (a_cap >= needed)
        return;

    size_t new_cap = a_cap == 0 ? needed : a_cap;
    while (new_cap < needed)
        new_cap = new_cap * 3 / 2 + 1;   /* grow factor */

    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* allocation failure: terminate */
    a_vals = tmp;
    a_cap  = new_cap;
}

/* Ensure we have at least 'n' primes in prime_ints                */
static void
ensure_prime_count(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* free previous prime array if any */
    free(prime_ints);
    sieve_first_n_primes(n, &prime_ints, &prime_cnt);
    if (prime_cnt < (size_t)n)
        abort();                /* sanity check */
}

/* Main sequence function                                                  */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* out of bounds; not used for OEIS offset 1 */

    /* Make sure primes up to index n are available */
    ensure_prime_count(n);

    /* Extend the cache for a_vals to hold at least n entries */
    ensure_a_capacity((size_t)n);

    /* Compute any missing values up to n (1‑based index)           */
    for (size_t i = a_used; i < (size_t)n; ++i) {
        uint32_t p = prime_ints[i];       /* current prime p_i (1‑based) */
        seq_t count = (seq_t)0;

        for (size_t j = 0; j < i; ++j) {
            /* test whether p mod q == 4 with q = prime_ints[j] */
            if ((p % prime_ints[j]) == 4)
                ++count;
        }
        a_vals[i] = count;
    }
    if (a_used < (size_t)n)
        a_used = (size_t)n;

    return a_vals[(size_t)n - 1];
}
