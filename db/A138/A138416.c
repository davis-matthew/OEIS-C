/*
 * A138416  a(n) = (p^3 - p^2)/2, where p = prime(n).
 * https://oeis.org/A138416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p^3 - p^2)/2, where p = prime(n)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 168, u64 n <= 168,
 *            u128 n <= 168, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *cached_primes = NULL;
static idx_t cached_n = 0;

/* Compute the nth prime (1‑based) and cache primes up to n */
static uint32_t get_nth_prime(idx_t n)
{
    if (n > cached_n)
    {
        uint32_t *tmp = NULL;
        size_t cnt = 0;
        sieve_first_n_primes(n, &tmp, &cnt);
        free(cached_primes);            /* free old cache if any */
        cached_primes = tmp;
        cached_n = n;
        /* cnt == n */
    }
    return cached_primes[(size_t)(n - 1)];
}

seq_t a(idx_t n)
{
    uint32_t p = get_nth_prime(n);
    seq_t p_seq = (seq_t)p;
    seq_t pp = p_seq * p_seq;          /* p^2 */
    seq_t res = pp * (p_seq - 1) / 2;   /* (p^3 - p^2)/2 = p^2*(p-1)/2 */
    return res;
}
