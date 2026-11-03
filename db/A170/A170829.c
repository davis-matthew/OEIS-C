/*
 * A170829  a(n) = gcd(Catalan(n), n!).
 * https://oeis.org/A170829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( Catalan(n), n! )
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 45 (2026-11-01)
 * Helpers:   build_primes_upto, catalan, gcd, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper functions supplied elsewhere */
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/catalan.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

/* exponent of prime p in m! */
static inline idx_t exp_fact(idx_t m, uint64_t p)
{
    idx_t e = 0;
    while (m > 0) {
        m /= p;
        e += m;
    }
    return e;
}

/* static cache of primes */
static uint64_t *primes_cache = NULL;
static size_t primes_count = 0;
static idx_t primes_limit = 0;

seq_t a(idx_t n)
{
    /* ensure primes up to n+1 are available */
    if (primes_limit < n + 1) {
        if (primes_cache != NULL) {
            free(primes_cache);
            primes_cache = NULL;
        }
        build_primes_upto(n + 1, &primes_cache, &primes_count);
        primes_limit = n + 1;
    }

    seq_t result = seq_one();

    for (size_t i = 0; i < primes_count && primes_cache[i] <= n + 1; ++i) {
        uint64_t p = primes_cache[i];

        /* exponents in factorials */
        idx_t exp_n_fact   = exp_fact(n, p);
        idx_t exp_2n_fact  = exp_fact(2 * n, p);
        idx_t exp_binom    = exp_2n_fact - 2 * exp_n_fact;               /* exponent in C(2n,n) */

        /* subtract exponent of p in (n+1) to get exponent in Catalan */
        idx_t exp_n1 = 0;
        idx_t m = n + 1;
        while (m % p == 0) {
            m /= p;
            exp_n1++;
        }
        idx_t exp_catalan = exp_binom - exp_n1;

        /* exponent in gcd = min(exp_catalan, exp_n_fact) */
        idx_t exp_gcd = exp_catalan < exp_n_fact ? exp_catalan : exp_n_fact;
        if (exp_gcd > 0) {
            seq_t powp = ipow_idx((seq_t)p, exp_gcd);
            result = seq_mul(result, powp);
        }
    }

    return result;
}
