/*
 * A235189  Number of ways to write n = (1 + (n mod 2))*p + q with p < n/2
 *          such that p, q and prime(p) - p + 1 are all prime.
 * https://oeis.org/A235189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ p < n/2 : (1+(n mod 2))*p + q = n, p, q, and
 *            prime(p)-p+1 are all prime }|
 * Range:     u8 n <= 426, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a static prime table to keep all primes
   needed for the decomposition.  The table is extended to cover at
   least the first needed count of primes.  The function relies on
   the utilities:
//
//  - build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt)
//         generates all primes ≤ limit and returns them in an array
//         of uint64_t’s together with their count.
//
//  - is_prime_small(seq_t n)
//         returns true if n fits in a machine word and is prime.
//
//  All other utilities are part of the build environment and need
//  no additional includes. */

/* Include standard headers needed for the implementation. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"

/* Static storage for the prime table. */
static uint64_t *primes_global = NULL;
static size_t   primes_global_cnt = 0;

/* Expand primes_global so that it holds at least 'need' primes.  The
   number of primes ≤ x is approximately x / log x; using this
   approximation we choose an appropriate upper bound for the sieve. */
static void
ensure_primes_cnt(size_t need)
{
    if (primes_global_cnt >= need)
        return;

    /* Estimate a sieve limit that contains at least 'need' primes. */
    double d = (double)need;
    idx_t  lim;
    if (need < 6) {
        lim = 15;
    } else {
        /* x/log x ≈ need  -->  x ≈ need * (log need + log log need) */
        lim = (idx_t)(d * (log(d) + log(log(d))) + 3.0);
    }
    if (lim < 15)
        lim = 15;

    /* Increase the limit until the produced list contains enough primes. */
    while (1) {
        uint64_t *tmp = NULL;
        size_t    tmp_cnt = 0;
        build_primes_upto(lim, &tmp, &tmp_cnt);
        if (tmp_cnt >= need) {
            free(primes_global);
            primes_global = tmp;
            primes_global_cnt = tmp_cnt;
            break;
        }
        free(tmp);
        lim = (idx_t)((double)lim * 2.0);
    }
}

/* Main OEIS entry function.  The argument n is the (1‑based) index. */
seq_t
a(idx_t n)
{
    /* p must satisfy p < n/2 */
    idx_t p_max = (n - 1) / 2;
    if (p_max < 2)
        return (seq_t)0;

    /* Ensure we have at least p_max primes (i.e. a table up to the
       p_max‑th prime). */
    ensure_primes_cnt((size_t)p_max);

    /* Convert components to seq_t before arithmetic to avoid
       mixed‑type problems. */
    seq_t sn   = (seq_t)n;
    seq_t mult = (n & 1ULL) ? (seq_t)2 : (seq_t)1;
    seq_t count = 0;

    /* Iterate through all primes p with p <= p_max. */
    for (size_t i = 0; i < primes_global_cnt; ++i) {
        uint64_t p = primes_global[i];
        if (p > (uint64_t)p_max)
            break;

        /* prime(p)  – the p-th prime. */
        seq_t prime_of_p = (seq_t)primes_global[p - 1];
        seq_t t = prime_of_p - (seq_t)p + 1;
        if (t < (seq_t)2 || !is_prime_small(t))
            continue;

        seq_t q = sn - mult * (seq_t)p;
        if (q < (seq_t)2 || !is_prime_small(q))
            continue;

        ++count;
    }
    return count;
}
