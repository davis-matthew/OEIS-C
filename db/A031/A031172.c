/*
 * A031172  a(n) = prime(n+10) - prime(n).
 * https://oeis.org/A031172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+10) - prime(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

/* Static cache for primes */
static idx_t          max_prime_needed = 0;
static uint32_t      *prime_cache      = NULL;
static size_t        prime_count       = 0;

/* Ensure that the cache contains at least the first `upto` primes */
static void ensure_primes(idx_t upto)
{
    if (upto <= max_prime_needed)
        return;                       /* already have enough */

    /* Free old cache if it exists */
    if (prime_cache != NULL)
        free(prime_cache);

    /* Build new cache */
    uint32_t *tmp = NULL;
    size_t   cnt  = 0;
    sieve_first_n_primes(upto, &tmp, &cnt);

    prime_cache   = tmp;
    prime_count   = cnt;
    max_prime_needed = upto;
}

/* OEIS sequence A031172 */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* behaviour for n<1 is irrelevant */

    idx_t need = n + 10;                    /* need prime at index n+10 */
    ensure_primes(need);                    /* generate primes up to need */

    /* Array indices are 0‑based, whereas OEIS indices are 1‑based */
    seq_t p_n    = (seq_t)prime_cache[n - 1];
    seq_t p_need = (seq_t)prime_cache[need - 1];

    return p_need - p_n;
}
