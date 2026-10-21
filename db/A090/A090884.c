/*
 * A090884  a(n) is the derivative of n via transport of structure from
 *          polynomials. Completely multiplicative with a(2) = 1,
 *          a(prime(i+1)) = prime(i)^i for i > 0.
 * https://oeis.org/A090884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = product over prime factors p_i of n with exponent e_i and
 *            index i>0 of (p_{i-1})^(e_i * i). For n = 1, a(1) = 1. The prime
 *            sequence starts with p_0 = 2
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 78,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, seq_pow_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow_fast.h"

static uint64_t *prime_list = NULL;   /* primes in ascending order, up to prime_limit */
static size_t     prime_cnt   = 0;     /* number of primes currently stored */
static uint64_t   prime_limit = 0;     /* largest prime currently stored */

/* Ensure prime_list contains all primes <= limit.  Replace old list if necessary. */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_limit >= limit) return;

    uint64_t *new_list = NULL;
    size_t    new_cnt  = 0;

    build_primes_upto(limit, &new_list, &new_cnt);

    if (prime_list) free(prime_list);
    prime_list   = new_list;
    prime_cnt    = new_cnt;
    prime_limit  = limit;
}

/* Binary search: return index of prime p in prime_list (p is guaranteed to be prime). */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* prime_list[lo] == p */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;     /* undefined for n == 0 */
    if (n == 1) return (seq_t)1;    /* a(1) = 1 */

    /* Factor n.  For 64‑bit n there can be at most 64 distinct prime factors. */
    uint64_t pf[64];          /* prime factors */
    uint64_t exps[64];        /* corresponding exponents */
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, pf, exps, &cnt);

    /* Determine the largest prime factor to know how many primes to pre‑compute. */
    uint64_t max_prime = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (pf[i] > max_prime) max_prime = pf[i];

    if (max_prime > 1)
        ensure_primes_upto(max_prime);

    seq_t result = (seq_t)1;

    /* Compute the product over all factors with prime index > 0. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = pf[i];
        uint64_t e     = exps[i];

        if (prime == 2) continue;          /* index 0 gives no base prime */

        size_t idx   = prime_index(prime);      /* i = index of prime in the prime sequence (>=1) */
        seq_t  base  = prime_list[idx - 1];      /* p_{i‑1} */
        idx_t  power = e * idx;                  /* exponent = e_i * i */

        seq_t term = seq_pow_fast(base, power);
        result = result * term;
    }

    return result;
}
