/*
 * A230140  Number of ways to write n = x + y + z with 0 < x <= y <= z such
 *          that 6*x-1, 6*y-1, 6*z-1 are among those primes p (terms of
 *          A230138) with p + 2 and 2*p - 5 also prime.
 * https://oeis.org/A230140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5659, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A230140.
   a(n) = number of ways to write n = x + y + z with
   0 < x ≤ y ≤ z such that 6*x-1, 6*y-1, 6*z-1 are primes p with
   p+2 and 2*p-5 also prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of helper routine. */
static void build_ok(idx_t max_n);

/* Static storage for the indicator array: ok_arr[k] == 1 if 6k-1 is a
   prime p with p+2 and 2p-5 also prime, 0 otherwise.  The array is
   indexed from 1 up to the current limit.  ok_arr[0] is unused. */
static uint8_t *ok_arr  = NULL;   /* indexed from 1 up to current limit */
static idx_t    ok_size = 0;      /* current size of ok_arr (max_k stored) */

/* Build ok_arr up to max_n.  Called from a() when a larger n is requested. */
static void build_ok(idx_t max_n)
{
    if (max_n <= ok_size)            /* already built up to this limit */
        return;

    /* Allocate a new array, freeing the old one if it existed. */
    if (ok_arr != NULL)
        free(ok_arr);
    ok_arr = (uint8_t *)calloc((size_t)(max_n + 1), sizeof(uint8_t));
    if (ok_arr == NULL)
        return;                      /* allocation failure → no data */

    /* We need to test primes up to 2p-5 where p ≤ 6·max_n-1,
       i.e. up to 12·max_n+5.  (The upper bound for p itself is 6·max_n-1.) */
    idx_t sieve_limit = 12 * max_n + 5;
    bool *isprime    = (bool *)calloc((size_t)sieve_limit, sizeof(bool));
    if (isprime == NULL) {          /* allocation failure → abort */
        free(ok_arr);
        ok_arr = NULL;
        return;
    }

    /* Initialise all numbers as potentially prime. */
    for (size_t i = 0; i < (size_t)sieve_limit; ++i)
        isprime[i] = true;
    isprime[0] = isprime[1] = false;

    /* Classical sieve of Eratosthenes. */
    for (idx_t i = 2; i * i < sieve_limit; ++i) {
        if (!isprime[i])
            continue;
        for (idx_t j = i * i; j < sieve_limit; j += i)
            isprime[j] = false;
    }

    /* Mark those k for which 6k-1 is a prime with the required
       neighbouring primes. */
    idx_t max_p = 6 * max_n - 1;   /* largest possible 6k-1 with k <= max_n */
    for (idx_t p = 5; p <= max_p; ++p) {        /* p ≥ 5 is the first 6k-1 prime */
        if (!isprime[p] || p % 6 != 5)
            continue;

        /* p+2 and 2p-5 are within the sieve bounds because
           p <= max_p = 6·max_n-1. */
        if (!isprime[p + 2] || !isprime[2 * p - 5])
            continue;

        idx_t k = (p + 1) / 6;              /* 6k-1 = p */
        if (k <= max_n)
            ok_arr[k] = 1;
    }

    free(isprime);
    ok_size = max_n;
}

/* Main sequence function, with external linkage. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Build the indicator array up to at least n. */
    static idx_t max_n_seen = 0;
    if (n > max_n_seen) {
        build_ok(n);
        max_n_seen = n;
    }
    if (ok_arr == NULL)          /* allocation failed in build_ok */
        return (seq_t)0;

    seq_t count = 0;

    for (idx_t x = 1; x <= n; ++x) {
        if (!ok_arr[x])
            continue;

        idx_t rem = n - x;                   /* remaining sum for y + z */
        idx_t y_min = x;
        idx_t y_max = rem / 2;               /* ensures y ≤ z */

        for (idx_t y = y_min; y <= y_max; ++y) {
            if (!ok_arr[y])
                continue;
            idx_t z = rem - y;               /* z ≥ y by construction */
            if (ok_arr[z])
                ++count;
        }
    }

    return count;
}
