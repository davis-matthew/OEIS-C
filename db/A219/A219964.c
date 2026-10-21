/*
 * A219964  a(n) = product(i >= 0, (P(n, i)/P(n-1, i))^(2^i)) where P(n, i) =
 *          product(p prime, n/2^(i+1) < p <= n/2^i).
 * https://oeis.org/A219964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i≥0} (P(n,i)/P(n-1,i))^{2ⁱ}, where P(n,i) = ∏_{prime
 *            p, n/2^{i+1} < p ≤ n/2^i} p. This is equivalent to a(n) =
 *            A220027(n)/A220027(n-1)
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 127, u64 n <= 255,
 *            u128 n <= 511, big any n
 * Helpers:   build_primes_upto, floor_log2_uint64, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_log2_uint64.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"

/* Cache of primes up to the largest n requested. */
static uint64_t *prime_list = NULL;   /* array of primes (uint64_t) */
static size_t prime_cnt   = 0;        /* number of primes in array     */
static idx_t  prime_limit = 0;        /* maximum n for which primes are built */

/* Ensure that prime_list contains all primes ≤ n.  Rebuild only when n grows. */
static void
ensure_primes(idx_t n)
{
    if (n <= prime_limit)
        return;                         /* already sufficient */

    /* Rebuild: free previous allocation (if any). */
    if (prime_list != NULL)
        free(prime_list);

    build_primes_upto(n, &prime_list, &prime_cnt);
    prime_limit = n;
}

/* Compute a(n) according to the definition. */
seq_t
a(idx_t n)
{
    /* By definition a(0) = 1. */
    if (n == 0)
        return (seq_t)1;

    /* Ensure primes up to n are available. */
    ensure_primes(n);

    /* Result starts at 1. */
    seq_t result = (seq_t)1;

    /* For every prime p ≤ n, determine its contribution. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)n)
            break;

        /* i_n = floor(log2(n/p))  (n/p is integer division). */
        idx_t ith_n = floor_log2_uint64((n) / p);

        /* Compute exponent contribution 2^i_n. */
        idx_t exp_n = ((idx_t)1) << ith_n;

        /* Determine exponent contribution from (n-1) if applicable. */
        idx_t exp_m = 0;
        if (p <= (uint64_t)(n - 1)) {
            idx_t ith_m = floor_log2_uint64((n - 1) / p);
            exp_m = ((idx_t)1) << ith_m;
        }

        /* Net exponent for this prime. */
        idx_t exponent = exp_n - exp_m;

        /* Multiply result by p raised to this exponent. */
        seq_t powv = pow_uint(p, exponent);
        result = seq_mul(result, powv);
    }

    return result;
}
