/*
 * A234740  Sum of the eleventh powers of the first n primes.
 * https://oeis.org/A234740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k)^11 (OEIS A234740)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 16, u128 n <= 350,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 863 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_u64_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/mem.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Returns the n-th term of the sequence, with n >= 1.
 * The function is memoised and works for calls in any order. */
seq_t a(idx_t n)
{
    /* Memoised cumulative sums: cumulative[0] = 0, cumulative[i] = a(i) */
    static seq_t *cumulative = NULL;
    static size_t   cum_cap   = 0;
    static idx_t    max_n     = 0;

    /* Primes cache */
    static uint64_t *primes    = NULL;
    static size_t    primes_cnt = 0;

    /* Ensure we have at least n primes */
    if (primes_cnt < (size_t)n) {
        if (primes) free(primes);
        size_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &primes, &primes_cnt);
        while (primes_cnt < (size_t)n) {
            limit *= 2;
            free(primes);
            build_primes_upto(limit, &primes, &primes_cnt);
        }
    }

    /* Extend the cumulative array if needed */
    size_t needed = (size_t)n + 1;          /* indices 0..n */
    if (cum_cap < needed) {
        size_t new_cap = cum_cap ? cum_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = mem_resize(seq_t, cumulative, new_cap);
        if (!tmp) abort();
        cumulative = tmp;
        cum_cap = new_cap;
        cumulative[0] = 0;                  /* base case */
    }

    /* Compute sums for any new indices */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            uint64_t p = primes[(size_t)(i - 1)];   /* primes are 0‑based */
            seq_t p_pow = ipow_u64_idx(p, 11);
            cumulative[(size_t)i] = cumulative[(size_t)(i - 1)] + p_pow;
        }
        max_n = n;
    }

    return cumulative[(size_t)n];
}
