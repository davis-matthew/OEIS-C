/*
 * A133535  Sum of fourth powers of two consecutive primes.
 * https://oeis.org/A133535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n^4 + p_{n+1}^4 (sum of fourth powers of two
 *            consecutive primes)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* a(0) is not defined in the OEIS sequence; return 0 to keep
       a well‑defined return value for n < 1. */
    if (n < 1)
        return (seq_t)0;

    static uint64_t *primes = NULL;
    static size_t primes_count = 0;

    /* Make sure we have at least n+1 primes (indices 0 … n). */
    if (primes_count < (size_t)(n + 1))
    {
        /* free any previously allocated array */
        if (primes)
            free(primes);

        /* _prime_limit_for_idx returns an upper bound that guarantees
           at least the requested number of primes. */
        idx_t limit = _prime_limit_for_idx(n + 1);
        size_t cnt;

        build_primes_upto(limit, &primes, &cnt);
        primes_count = cnt;  /* primes_count >= n+1 from the above bound */
    }

    /* Fetch the required primes.  primes[0] == 2, primes[1] == 3, … */
    uint64_t p1 = primes[n - 1];
    uint64_t p2 = primes[n];

    /* Compute p1^4 and p2^4 in seq_t arithmetic. */
    seq_t term1 = (seq_t)p1;
    term1 *= p1;
    term1 *= p1;
    term1 *= p1;   /* term1 = p1^4 */

    seq_t term2 = (seq_t)p2;
    term2 *= p2;
    term2 *= p2;
    term2 *= p2;   /* term2 = p2^4 */

    return term1 + term2;
}
