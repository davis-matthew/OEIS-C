/*
 * A100810  a(n) = 0 if prime(n) + 2 = prime(n+1), otherwise 1.
 * https://oeis.org/A100810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if prime(n) + 2 = prime(n+1), otherwise 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a dynamic list of primes up to the
   greatest n requested.  It uses the helper function
   is_prime(uint64_t) to test primality.  The list is stored in
   a static array that grows as needed.  The function a(idx_t n)
   guarantees correct results for any n ≥ 1 independent of call
   order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t *primes = NULL;          /* array of primes, 0‑indexed   */
static idx_t      primes_count = 0;      /* number of primes stored     */
static size_t     primes_size  = 0;      /* allocated array capacity    */

/* Ensure that at least count primes are available.  We need n+1 primes
   for a(n), because a(n) compares p_n and p_{n+1}. */
static void
ensure_primes(idx_t count)
{
    if (primes_count >= count)
        return;

    /* Grow the array if necessary */
    if (primes_size == 0) {
        primes_size = 128;
        primes = (uint64_t *)malloc(primes_size * sizeof(uint64_t));
    } else {
        while (primes_size < (size_t)count) {
            primes_size <<= 1;
        }
        primes = (uint64_t *)realloc(primes, primes_size * sizeof(uint64_t));
    }

    /* Initial prime */
    if (primes_count == 0) {
        primes[0] = 2;
        primes_count = 1;
    }

    /* Generate subsequent primes */
    uint64_t candidate = (primes_count == 1) ? 3 : primes[primes_count - 1] + 2;
    while (primes_count < count) {
        if (is_prime(candidate)) {
            primes[primes_count++] = candidate;
        }
        candidate += 2; /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    /* a(n) uses primes[p_n] and primes[p_{n+1}]; need n+1 primes */
    ensure_primes(n + 1);

    /* p_n is at index n-1, p_{n+1} at index n */
    uint64_t pn   = primes[n - 1];
    uint64_t pn1  = primes[n];

    return pn1 == pn + 2 ? (seq_t)0 : (seq_t)1;
}
