/*
 * A056042  a(n) = n!/(k!)^2, where k is the largest number such that (k!)^2
 *          divides n!.
 * https://oeis.org/A056042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / (k!)^2, where k is the largest integer such that *
 *            (k!)^2 divides n!. * Algorithm: * 1. Compute the list of primes
 *            up to n (cached across calls). * 2. Binary‑search for the
 *            maximal k ∈ [0..n] satisfying the divisibility * condition using
 *            prime exponents in factorials. * 3. Compute the...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   build_primes, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Utility headers provided by the build environment */
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/seq_pow.h"        /* seq_t seq_pow(seq_t base, idx_t exp); */

/* Cached prime list */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure primes up to 'limit' are available */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    /* build_primes allocates a new array; reuse on each call */
    if (prime_list) free(prime_list);
    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Exponent of prime p in m! */
static idx_t fact_exp(idx_t m, idx_t p)
{
    idx_t exp = 0;
    for (idx_t pp = p; pp <= m; pp *= p) {
        exp += m / pp;
        if (pp > m / p) break;    /* avoid overflow of pp *= p */
    }
    return exp;
}

/* Does (k!)^2 divide n! ? */
static bool divides(idx_t n, idx_t k)
{
    if (k == 0) return true;      /* (0!)^2 = 1 */
    ensure_primes(k);
    for (size_t i = 0; i < prime_count && prime_list[i] <= k; ++i) {
        idx_t p   = prime_list[i];
        idx_t an  = fact_exp(n, p);
        idx_t ak  = fact_exp(k, p);
        if (2 * ak > an) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;    /* for safety; offset starts at 1 */

    /* Find maximal k via binary search */
    idx_t low  = 0;
    idx_t high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (divides(n, mid))
            low = mid;
        else
            high = mid - 1;
    }
    idx_t k = low;

    /* Compute a(n) = n! / (k!)^2 by prime factorization */
    seq_t result = 1;
    ensure_primes(n);
    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i) {
        idx_t p      = prime_list[i];
        idx_t exp_n  = fact_exp(n, p);
        idx_t exp_k  = fact_exp(k, p);
        idx_t delta  = exp_n - 2 * exp_k;
        if (delta > 0) {
            seq_t power = seq_pow((seq_t)p, delta);
            result = seq_mul(result, power);
        }
    }
    return result;
}
