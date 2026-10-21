/*
 * A092287  a(n) = Product_{j=1..n} Product_{k=1..n} gcd(j,k).
 * https://oeis.org/A092287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n} p^{ ∑_{k≥1} ⌊n/p^k⌋² }
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   build_primes_upto, pow_int, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This is the prime‑power factorisation given in the OEIS entry.) */
/* The code caches the list of primes up to the largest n seen so far
   and uses the provided big‑integer utilities to compute the product. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_int.h"

/* Cached list of primes up to the largest n encountered so far. */
static uint64_t *cached_primes = NULL;
static size_t    cached_prime_cnt   = 0;
static idx_t     cached_prime_lim   = 0;

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Base case: a(0) = 1. */
    if (n == 0ULL) {
        return (seq_t)1;
    }

    /* Extend the cached prime list if we need primes > cached limit. */
    if (n > cached_prime_lim) {
        uint64_t *pts = NULL;
        size_t cnt = 0;
        build_primes_upto(n, &pts, &cnt);
        /* Replace old cache. */
        if (cached_primes != NULL) {
            free(cached_primes);
        }
        cached_primes = pts;
        cached_prime_cnt = cnt;
        cached_prime_lim = n;
    }

    /* Result starts at 1 and accumulates the prime powers. */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cached_prime_cnt; ++i) {
        uint64_t p = cached_primes[i];
        /* Compute the exponent: sum_{k≥1} ⌊n/p^k⌋². */
        idx_t div = n / p;
        idx_t exponent = 0;
        while (div > 0) {
            exponent += div * div;
            div /= p;
        }
        if (exponent > 0) {
            seq_t term  = pow_int((seq_t)p, exponent);
            result = seq_mul(result, term);
        }
    }

    return result;
}
