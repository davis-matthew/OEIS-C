/*
 * A166329  Products of squares of 2 successive primes.
 * https://oeis.org/A166329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p_n * p_{n+1})^2 where p_k is the k-th prime
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 54, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL;  /* array of prime numbers (0‑based) */
static size_t prime_cnt = 0;        /* number of primes stored */

/* Ensure that prime_arr contains at least (n+1) primes. */
static void ensure_primes(idx_t n)
{
    /* We need primes up to index n (0‑based), i.e. n+1 primes. */
    if (prime_cnt >= (size_t)(n + 1))
        return;

    /* Use the helper to get a safe upper bound for the nth prime. */
    idx_t limit = _prime_limit_for_idx(n + 1);

    /* Re‑sieve if our estimate turned out too small. */
    while (1) {
        uint64_t *tmp   = NULL;
        size_t   tmp_cnt = 0;
        build_primes_upto(limit, &tmp, &tmp_cnt);

        if (tmp_cnt >= (size_t)(n + 1)) {
            /* Replace the old array. */
            if (prime_arr)
                free(prime_arr);
            prime_arr = tmp;
            prime_cnt = tmp_cnt;
            break;
        }
        /* Too few primes; free and increase the limit. */
        free(tmp);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; 0 is not defined */
        return (seq_t)0;

    /* Make sure we have the needed primes. */
    ensure_primes(n + 1);   /* need primes[0..n] */

    /* p_n is prime_arr[n-1], p_{n+1} is prime_arr[n] */
    uint32_t p1 = (uint32_t)prime_arr[n - 1];
    uint32_t p2 = (uint32_t)prime_arr[n];

    unsigned __int128 prod  = (unsigned __int128)p1 * (unsigned __int128)p2;
    unsigned __int128 square = prod * prod;

    return (seq_t)square;
}
