/*
 * A071215  Number of distinct prime factors of sum of 2 successive primes.
 * https://oeis.org/A071215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = omega(prime(n) + prime(n+1)) = number of distinct prime
 *            factors of the sum of the n‑th and (n+1)‑st primes
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function may be called in any order; it lazily caches a list
   of primes large enough for the requested n. It uses the provided
   helper functions for prime generation and prime‑factor counting. */

#include <stddef.h>                     /* size_t */
#include <stdlib.h>                     /* free */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/count_distinct_primes.h"

/* static cache of primes */
static uint64_t *prime_list   = NULL;   /* array of primes (uint64_t) */
static size_t   prime_count  = 0;       /* number of elements in prime_list */

/* Ensure that prime_list contains at least n+1 primes
   (indices 0 .. n inclusive). */
static void ensure_primes_upto_idx(idx_t n)
{
    if (prime_count > (size_t)n)
        return;                    /* already have enough */

    /* compute an upper bound for the (n+1)‑st prime */
    idx_t limit = _prime_limit_for_idx(n + 1);

    /* rebuild the prime list up to the required limit */
    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;
    build_primes_upto(limit, &new_list, &new_cnt);

    /* replace the old list (if any) and update the counter */
    free(prime_list);
    prime_list  = new_list;
    prime_count = new_cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1 – return 0 for safety */
        return 0;

    /* we need primes at indices n-1 and n (0‑based) */
    ensure_primes_upto_idx(n);          /* guarantees prime_list[n] exists */

    uint64_t p1 = prime_list[(size_t)(n - 1)];
    uint64_t p2 = prime_list[(size_t)n];
    uint64_t sum = p1 + p2;

    return count_distinct_primes(sum);
}
