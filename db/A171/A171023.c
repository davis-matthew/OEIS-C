/*
 * A171023  In the sequence of prime numbers, replace all the '5' digits with
 *          '0' and vice versa.
 * https://oeis.org/A171023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171023: In the sequence of prime numbers, replace all
 * the '5' digits with '0' and vice versa.  a(n) = transformed(prime_n).
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Estimate an upper bound on the n-th prime.  For n <= 10000 the bound
 * n*20+100 is more than enough (the 10000th prime is 104729). */
static idx_t estimate_limit(idx_t n)
{
    return n * 20 + 100;
}

/* Transform a prime by swapping 5↔0 in its decimal representation. */
static seq_t transform_prime(uint64_t p)
{
    /* Store digits (least‑significant first). */
    uint8_t digits[32];     /* 32 digits is more than enough for 64‑bit uint. */
    int len = 0;
    while (p > 0) {
        digits[len++] = (uint8_t)(p % 10);
        p /= 10;
    }
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned int d = digits[i];
        if (d == 5)
            d = 0;
        else if (d == 0)
            d = 5;
        result = result * 10 + d;
    }
    return result;
}

/* Cached list of primes. */
static bool   primes_initialized = false;
static idx_t  primes_limit       = 0;
static size_t primes_count       = 0;
static uint64_t *primes_array    = NULL;

/* Ensure that primes_array contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (!primes_initialized) {
        primes_limit = estimate_limit(n);
        build_primes_upto(primes_limit, &primes_array, &primes_count);
        primes_initialized = true;
        return;
    }

    /* If we already have enough primes, nothing to do. */
    if (n <= (idx_t)primes_count)
        return;

    /* Need more primes: rebuild with a larger limit. */
    if (primes_array)
        free(primes_array);
    primes_limit = estimate_limit(n);
    build_primes_upto(primes_limit, &primes_array, &primes_count);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for n=0, safe default */
    ensure_primes(n);       /* build primes up to at least n */
    uint64_t p = primes_array[n - 1];   /* nth prime (1‑based) */
    return transform_prime(p);
}
