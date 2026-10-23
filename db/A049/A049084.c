/*
 * A049084  a(n) = pi(n) if n is prime, otherwise 0.
 * https://oeis.org/A049084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Sequence: a(n) = pi(n) if n is prime, otherwise 0
   (pi is the prime‑counting function) */

static idx_t   _max_limit = 0;   /* largest n for which the arrays are valid */
static bool   *_is_prime  = NULL;
static seq_t *_pi_value  = NULL;

/* Build or extend a sieve and pi array up to at least 'limit' */
static void
_build_sieve(idx_t limit)
{
    /* Allocate new arrays of the required size */
    bool   *prime = (bool*)malloc((limit + 1) * sizeof(bool));
    seq_t  *pi_arr = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!prime || !pi_arr) {
        free(prime);
        free(pi_arr);
        return;   /* allocation failure – keep old state unchanged */
    }

    /* Initialise all entries to false */
    for (idx_t i = 0; i <= limit; ++i) prime[i] = false;

    /* Mark all numbers ≥ 2 as potential primes */
    for (idx_t i = 2; i <= limit; ++i) prime[i] = true;

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (!prime[p]) continue;
        for (idx_t m = p * p; m <= limit; m += p)
            prime[m] = false;
    }

    /* Build the prime‑counting array pi(n) */
    seq_t cnt = 0;
    pi_arr[0] = (seq_t)0;
    if (limit >= 1) pi_arr[1] = (seq_t)0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (prime[i]) ++cnt;
        pi_arr[i] = cnt;
    }

    /* Replace old data with the new arrays */
    free(_is_prime);
    free(_pi_value);
    _is_prime = prime;
    _pi_value = pi_arr;
    _max_limit = limit;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* out‑of‑range indices */
    if (n > _max_limit) _build_sieve(n);       /* make sure we have data up to n */

    if (!_is_prime[n])                         /* not prime → 0 */
        return (seq_t)0;
    return _pi_value[n];                       /* pi(n) when n is prime */
}
