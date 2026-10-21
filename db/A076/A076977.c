/*
 * A076977  Product of the largest prime divisors of composite numbers between
 *          successive primes.
 * https://oeis.org/A076977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p_n < m < p_{n+1}, m composite} LPD(m), where LPD(m)
 *            is the largest prime divisor of m and p_k is the k‑th prime.
 *            This implementation builds the primes on demand and factors each
 *            composite number on the fly
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 61, u128 n <= 188,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/build_primes_upto.h"

/* static cache of primes */
static uint64_t *primes        = NULL;
static size_t   primes_count   = 0;

/* ensure that at least 'needed' primes (0‑based index) are stored */
static void
ensure_primes(idx_t needed)
{
    if (primes_count >= (size_t)needed)
        return;

    /* a crude over‑estimate for the (needed‑th) prime:
       for n >= 10,  n·25 + 100 exceeds p_{n+1} */
    idx_t limit;
    if (needed <= 10)
        limit = 35;                /* covers the first 11 primes */
    else
        limit = needed * 25 + 100; /* safe for n up to many thousands */

    /* free any old cache */
    if (primes) {
        free(primes);
        primes = NULL;
        primes_count = 0;
    }

    /* build a new prime list up to 'limit' */
    build_primes_upto(limit, &primes, &primes_count);
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* before the offset – undefined */

    /* we need primes p_n and p_{n+1} (1‑based) => indices n‑1 and n */
    ensure_primes(n + 1);

    uint64_t p_prev = primes[n - 1];
    uint64_t p_next = primes[n];

    seq_t prod = (seq_t)1;
    for (uint64_t m = p_prev + 1; m < p_next; ++m) {
        if (is_prime(m))          /* skip primes themselves */
            continue;
        uint64_t lp = largest_prime_divisor(m);
        prod = prod * (seq_t)lp;   /* multiplication over seq_t */
    }
    return prod;
}
