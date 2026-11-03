/*
 * A230460  Prime(2*prime(n)).
 * https://oeis.org/A230460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*prime(n)) (OEIS A230460)
 * Range:     u8 n <= 9, u16 n <= 462, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 941 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

/* Static cache of primes */
static idx_t *primes = NULL;
static size_t prime_count = 0;

/* Ensure that at least `needed` primes are available in `primes`.  */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed)
        return;

    /* Start with an estimate of the upper bound for the needed-th prime. */
    idx_t lim = _prime_limit_for_idx(needed);
    while (1) {
        uint64_t *tmp_p = NULL;
        size_t cnt = 0;
        build_primes_upto(lim, &tmp_p, &cnt);

        if (cnt >= needed) {
            /* Replace old table.  The returned array is already
               malloc’ed with the correct element type (uint64_t). */
            free(primes);
            primes = (idx_t *)tmp_p;
            prime_count = cnt;
            break;
        }
        /* Not enough primes – double the limit and try again. */
        free(tmp_p);
        lim *= 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    /* Get the n-th prime. */
    ensure_primes(n);
    idx_t p_n = primes[n - 1];

    /* 2 * p_n is the target index (1‑based). */
    idx_t target = p_n << 1;            /* 2 * p_n */

    if (target == 0)                    /* overflow guard, should not happen */
        return 0;

    /* Ensure we have enough primes for the target index. */
    ensure_primes(target);

    /* Return the prime at that index. */
    return (seq_t)primes[target - 1];
}
