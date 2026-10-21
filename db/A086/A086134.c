/*
 * A086134  Smallest prime factor of arithmetic derivative of n or a(n)=0 if
 *          no such prime exists.
 * https://oeis.org/A086134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 317, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, factor_u64, isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A086134: smallest prime factor of
   the arithmetic derivative of n (a(n)=0 if such a prime does not exist) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/build_primes.h"
#include "../utils/isqrt_seq.h"

/* Compute the smallest prime factor of x, returning 0 if x ≤ 1. */
static seq_t _spf_big(seq_t x)
{
    if (x <= 1) return 0;

    /* Even numbers */
    if ((x & 1) == 0) return 2;

    /* Square‑root of x for the sieve limit */
    idx_t limit = (idx_t)isqrt_seq(x);

    /* Cached list of primes up to the current limit */
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 0;

    if (limit > prime_limit) {
        idx_t *new_list = NULL;
        size_t new_count = 0;
        if (prime_list) free(prime_list);
        build_primes(limit, &new_list, &new_count);
        prime_list   = new_list;
        prime_count  = new_count;
        prime_limit  = limit;
    }

    /* Trial division with cached primes */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        if (x % p == 0) return p;
    }

    /* If no divisor found, x is prime */
    return x;
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1) return 0;

    /* factor n using the utility routine */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* arithmetic derivative: d(n) = Σ (n / p_i) * e_i */
    seq_t sn = (seq_t)n;
    seq_t der = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (sn / (seq_t)pr[i]) * (seq_t)ex[i];
        der += term;
    }

    /* If the derivative is 0 or 1, return 0 */
    if (der <= 1) return 0;

    /* smallest prime factor of the derivative */
    return _spf_big(der);
}
