/*
 * A068110  Denominators of coefficients in J0(i*sqrt(x))^2 power series where
 *          J0 denotes the ordinary Bessel function of order 0.
 * https://oeis.org/A068110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of coefficient (2n)!/(n!)^4/2^(2n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   v_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, a(0) = 1. */

/* NOTE: This code uses only integer arithmetic and relies on the
   build's definition of seq_t and idx_t. The pragma below ensures
   compatibility with C++17 where seq_t may be an arbitrary‑precision
   class. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/v_factorial.h"

/* Global primes table */
static int *primes = NULL;
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/* Ensure that the list of primes contains all primes <= limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;

    /* Simple sieve up to limit */
    bool *comp = (bool *)calloc(limit + 1, 1);
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!comp[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                comp[m] = true;
        }
    }
    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            ++cnt;

    int *new_pr = (int *)malloc(cnt * sizeof(int));
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!comp[i])
            new_pr[idx++] = (int)i;

    free(comp);
    free(primes);
    primes = new_pr;
    primes_cnt = cnt;
    primes_limit = limit;
}

/* Fast exponentiation in seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Cache for previously computed terms */
static seq_t *term_cache = NULL;
static size_t cache_size = 0;
static idx_t cache_max_computed = 0;

static void ensure_cache(size_t needed)
{
    if (needed <= cache_size)
        return;
    seq_t *new_cache = (seq_t *)realloc(term_cache, needed * sizeof(seq_t));
    term_cache = new_cache;
    cache_size = needed;
}

/* Compute a single term a(n) */
static seq_t compute_term(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_primes(2 * n);

    seq_t denom = 1;
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] <= 2 * n; ++i) {
        idx_t p = (idx_t)primes[i];
        idx_t vp_num = v_factorial(2 * n, p);
        idx_t vp_den = 4 * v_factorial(n, p) + (p == 2 ? 2 * n : 0);
        if (vp_den > vp_num) {
            idx_t e = vp_den - vp_num;
            seq_t powval = pow_seq_t((seq_t)p, e);
            denom *= powval;
        }
    }
    return denom;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) {
        ensure_cache(1);
        term_cache[0] = (seq_t)1;
        cache_max_computed = 0;
        return (seq_t)1;
    }

    /* Expand cache if needed */
    ensure_cache(n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = cache_max_computed + 1; i <= n; ++i)
        term_cache[i] = compute_term(i);

    cache_max_computed = n;
    return term_cache[n];
}
