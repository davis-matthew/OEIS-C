/*
 * A031378  a(n) = prime(4*n - 2).
 * https://oeis.org/A031378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(4*n - 2) (the (4*n-2)-th prime)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_primes.h"

/* Static cache of primes: primes_arr holds primes[0] = 2, ... */
static idx_t *primes_arr = NULL;
static size_t primes_arr_cnt = 0;
static idx_t sieve_limit = 0;

/* Ensure that primes_arr contains at least the first `needed` primes. */
static void
ensure_primes_upto(idx_t needed)
{
    if (primes_arr_cnt >= needed)
        return;

    /* Start with a small limit and double until we have enough primes. */
    idx_t limit = (sieve_limit > 0) ? sieve_limit : 100;

    while (primes_arr_cnt < needed) {
        /* Increase the limit. */
        limit *= 2;
        if (primes_arr)
            free(primes_arr);
        sieve_primes(limit, &primes_arr, &primes_arr_cnt);
        sieve_limit = limit;
    }
}

/* Return the `n`-th term of the sequence (n ≥ 1). */
seq_t
a(idx_t n)
{
    /* Sequence offset starts at 1 */
    idx_t index = 4 * n - 2;            /* index into prime list (1‑based) */
    ensure_primes_upto(index);
    return (seq_t)primes_arr[index - 1];
}
