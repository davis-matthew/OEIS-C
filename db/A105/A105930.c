/*
 * A105930  Starting position of the n-th prime in the almost-natural numbers
 *          sequence A007376.
 * https://oeis.org/A105930
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = position of the first digit of the n-th prime written in
 *            the digit sequence 123456789101112
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/estimate_prime_upper_bound.h"   /* provides estimate_prime_upper_bound */

/* Forward declarations of utility types (supplied by the build). */
extern idx_t estimate_prime_upper_bound(idx_t n);

/* Minimal digit counter for 64‑bit integers. */
static seq_t digit_count_uint64(uint64_t x)
{
    seq_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* Prime storage: primes_arr[0] = 2, primes_arr[1] = 3, ... */
static uint64_t *primes_arr = NULL;
static size_t   primes_cnt = 0;        /* number of primes stored */

/* Prefix array: pref_arr[i] = total number of decimal digits in 1..i. */
static seq_t *pref_arr = NULL;
static size_t  pref_cap  = 0;          /* pref_cap >= maximum i currently stored */

/* Highest integer that has been sieved so far. */
static idx_t current_limit = 0;

/* Sieve of Eratosthenes up to `limit`, storing primes and prefix counts. */
static void build_primes_and_pref(idx_t limit)
{
    bool *is_prime = (bool *)malloc((size_t)limit + 1);
    if (!is_prime) exit(1);
    memset(is_prime, 1, (size_t)limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;

    /* Count primes. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;

    /* Store primes. */
    uint64_t *new_primes = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!new_primes) exit(1);
    size_t o = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[o++] = (uint64_t)i;

    /* Replace old arrays. */
    free(primes_arr);
    primes_arr = new_primes;
    primes_cnt = count;

    /* Build prefix digit counts. */
    seq_t *new_pref = (seq_t *)malloc(((size_t)limit + 1) * sizeof(seq_t));
    if (!new_pref) exit(1);
    new_pref[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_pref[i] = new_pref[i - 1] + digit_count_uint64(i);

    free(pref_arr);
    pref_arr = new_pref;
    pref_cap = (size_t)limit;

    free(is_prime);
    current_limit = limit;
}

/* Ensure we have at least (needed_index + 1) primes available. */
static void ensure_primes_up_to(idx_t needed_index)
{
    if (needed_index + 1 <= primes_cnt) return;

    /* Determine an upper bound for the required prime. */
    idx_t limit = estimate_prime_upper_bound(needed_index + 1) + 10;
    if (limit <= current_limit) limit = current_limit + 10;

    build_primes_and_pref(limit);
}

/* --------------------------------------------------------------- */
/*                     Main sequence function                      */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* undefined below the offset */

    /* n maps to the (n-2)-th prime (0-based). */
    idx_t prime_index = n - 2;
    ensure_primes_up_to(prime_index);

    uint64_t prime = primes_arr[prime_index];
    /* Position = digits up to prime-1 + 1. */
    seq_t pos = pref_arr[prime - 1] + 1;
    return pos;
}
