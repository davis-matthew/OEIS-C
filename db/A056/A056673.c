/*
 * A056673  Number of unitary and squarefree divisors of binomial(n,
 *          floor(n/2)). Also the number of divisors of the powerfree part of
 *          A001405(n), A056060(n).
 * https://oeis.org/A056673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^( number of primes p such that the exponent of p in
 *            binomial(n, floor(n/2)) equals 1 )
 * Range:     u8 n <= 34, u16 n <= 98, u32 n <= 218, u64 n <= 511,
 *            u128 n <= 1174, big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"

/* Cached prime list up to the largest n seen. */
static uint64_t   *prime_list  = NULL;
static size_t     prime_cnt   = 0;
static idx_t      prime_limit = 0;

/* Ensure that all primes ≤ n are precomputed.  Rebuild if the limit increases. */
static void ensure_primes_upto(idx_t n)
{
    if (prime_limit >= n && prime_list) return;
    if (prime_list) { free(prime_list); prime_list = NULL; }
    build_primes_upto(n, &prime_list, &prime_cnt);
    prime_limit = n;
}

/* Count the primes that occur in the binomial with exponent exactly 1. */
static idx_t count_once_primes(idx_t n)
{
    idx_t k  = n >> 1;          /* floor(n/2)   */
    idx_t nk = n - k;           /* ceil(n/2)    */
    idx_t once = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n) break;
        idx_t exp = 0;
        for (idx_t v = p; v <= n; v *= p) exp += n / v;
        for (idx_t v = p; v <= k;  v *= p) exp -= k / v;
        for (idx_t v = p; v <= nk; v *= p) exp -= nk / v;
        if (exp == 1) ++once;
    }
    return once;
}

/* External linkage: the sequence function required by the task. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* safety for offset handling */
    ensure_primes_upto(n);
    idx_t once = count_once_primes(n);
    return seq_pow((seq_t)2, once);
}
