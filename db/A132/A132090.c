/*
 * A132090  a(n) = pi(pi(n)), where pi = A000720.
 * https://oeis.org/A132090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(pi(n)), where pi = A000720, offset 1. The function
 *            uses an incremental sieve that stores all prime flags and the
 *            prime‑count array up to the current maximum limit
 * Range:     u8 n <= 2373, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for the sieve */
static idx_t     _max_sieved = 0;
static bool      *_is_prime   = NULL;
static seq_t     *_pi_arr     = NULL;

/* Ensure that primes are known up to at least 'limit'. */
static void _ensure_sieve(idx_t limit)
{
    if (limit <= _max_sieved)
        return;

    /* Reallocate storage for the new limit (+1 for zero). */
    idx_t   sz = limit + 1;
    bool   *new_is_prime = (bool *)realloc(_is_prime, sz * sizeof(bool));
    if (new_is_prime == NULL) abort();
    seq_t   *new_pi      = (seq_t *)realloc(_pi_arr,   sz * sizeof(seq_t));
    if (new_pi == NULL) abort();

    /* Initialise all entries as potential primes */
    for (idx_t i = 0; i < sz; ++i)
        new_is_prime[i] = true;
    if (sz > 0) new_is_prime[0] = false;
    if (sz > 1) new_is_prime[1] = false;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (new_is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                new_is_prime[j] = false;

    /* Build the prime‑count table */
    seq_t count = 0;
    new_pi[0] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (new_is_prime[i])
            ++count;
        new_pi[i] = count;
    }

    /* Commit the new data */
    _is_prime   = new_is_prime;
    _pi_arr     = new_pi;
    _max_sieved = limit;
}

/* Sequence entry a(n) = pi(pi(n)). */
seq_t a(idx_t n)
{
    if (n == 0)              /* guard against out‑of‑bounds: offset is 1 */
        return 0;

    _ensure_sieve(n);
    seq_t pi_n = _pi_arr[n];
    idx_t idx  = (idx_t)pi_n;     /* pi(n) fits in idx_t for all n we handle */

    /* pi(pi(n)) is within the range already sieved (pi(n) <= n). */
    _ensure_sieve(idx);
    return _pi_arr[idx];
}
