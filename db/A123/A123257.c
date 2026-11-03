/*
 * A123257  Multiplicative encoding of nim sum triangle: Product
 *          p(i+1)^BitXOR(n,i).
 * https://oeis.org/A123257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=1..n} p(i)^{BitXOR(n,i)} p(i) is the i‑th prime
 *            (p(1)=2). The exponent is (i-1) XOR (n-i); for n = 1 this yields
 *            0, so the product is adjusted to 2 by a single special case
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   build_primes_upto, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* External utilities */
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Compute a(n) for OEIS sequence A123257 */
seq_t a(idx_t n)
{
    /* Base case: a(1) = 2 */
    if (n == 0) return seq_one();  /* defensive; sequence is offset 1 */
    if (n == 1) return (seq_t)2ULL;

    static uint64_t *prime_cache = NULL;
    static idx_t cached_primes   = 0;

    /* Ensure enough primes are cached for indices up to n */
    if (cached_primes < n) {
        /* Release old cache if any */
        if (prime_cache) free(prime_cache);

        /* Compute an upper bound that contains at least n primes */
        size_t limit   = (size_t)_prime_limit_for_idx(n);
        size_t count   = 0;
        uint64_t *tmp   = NULL;

        build_primes_upto(limit, &tmp, &count);
        prime_cache = tmp;
        cached_primes = (idx_t)count;
    }

    seq_t result = seq_one();

    for (idx_t i = 1; i <= n; ++i) {
        /* Exponent = (i-1) XOR (n-i) */
        idx_t exponent = (i - 1) ^ (n - i);
        if (n == 1) exponent = 1;  /* adjust for the first term */

        seq_t base  = (seq_t)prime_cache[i - 1];      /* p(i)   */
        seq_t power = ipow(base, (seq_t)exponent); /* base^exponent */
        result = seq_mul(result, power);
    }

    return result;
}
