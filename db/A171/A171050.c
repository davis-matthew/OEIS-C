/*
 * A171050  In the sequence of prime numbers, replace all the '9' digits with
 *          '1' and vice versa.
 * https://oeis.org/A171050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = replace digits 1<->9 in the nth prime (offset 1)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 997 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Swap 1↔9 in the decimal representation of v, keeping digit order. */
static inline seq_t transform_digits(seq_t v)
{
    seq_t res = 0;
    seq_t mult = 1;
    while (v > 0) {
        seq_t d = v % 10;
        if (d == 1)
            d = 9;
        else if (d == 9)
            d = 1;
        res += d * mult;
        mult *= 10;
        v /= 10;
    }
    return res;
}

static uint64_t *s_primes = NULL;
static size_t    s_prcnt  = 0;
static idx_t     s_limit  = 0;

/* Ensure at least n primes are available. */
static void ensure_primes(idx_t n)
{
    idx_t lim = _prime_limit_for_idx(n);
    if (s_prcnt >= (size_t)n && lim <= s_limit)
        return;                      /* Current cache is sufficient. */

    /* Rebuild cache if necessary. */
    if (s_primes) {
        free(s_primes);
        s_primes = NULL;
    }
    build_primes_upto(lim, &s_primes, &s_prcnt);
    s_limit = lim;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* Offset is 1; below offset is undefined. */

    ensure_primes(n);
    return transform_digits((seq_t)s_primes[n - 1]);
}
