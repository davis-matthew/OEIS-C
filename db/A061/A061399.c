/*
 * A061399  Number of nonsquarefree integers between primes prime(n) and
 *          prime(n+1).
 * https://oeis.org/A061399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonsquarefree integers in the open interval
 *            (prime(n), prime(n+1))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 932 (2026-11-03)
 * Helpers:   build_primes, build_spf, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/build_spf.h"
#include "../utils/mem.h"

/* Upper bound for the n‑th prime (n ≥ 6).  Useful for the sieve limit. */
static size_t estimate_prime_upper_bound(size_t n) {
    if (n < 6) return 15;                /* primes up to 13 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (size_t)(bound) + 3;          /* safety margin */
}

/* static storage for sieve data */
static idx_t *primes = NULL;         /* prime list (0‑based)           */
static size_t prime_cnt = 0;          /* number of primes stored       */
static idx_t *spf = NULL;            /* smallest prime factor array   */
static bool *squarefree = NULL;      /* flags whether i is squarefree */
static seq_t *pref = NULL;           /* prefix count of nonsquarefree */

/* highest n for which data structures have been built (n+1 primes)   */
static idx_t max_n_computed = 0;

/* build or rebuild all tables large enough to cover at least n_req primes */
static void init_for_n(idx_t n_req)
{
    if (n_req <= max_n_computed)
        return;            /* already have enough data */

    /* free old tables if any */
    if (primes) free(primes);
    if (spf)    free(spf);
    if (squarefree) free(squarefree);
    if (pref)    mem_free(pref);

    primes = NULL;  spf = NULL;  squarefree = NULL;  pref = NULL;

    /* estimate a limit large enough to contain n_req‑th prime */
    idx_t limit = estimate_prime_upper_bound(n_req) + 100;
    while (1) {
        /* build primes up to limit */
        build_primes(limit, &primes, &prime_cnt);
        if (prime_cnt >= n_req)
            break;          /* enough primes            */
        /* otherwise enlarge and try again */
        limit <<= 1;
        free(primes);
    }

    /* smallest prime factor up to 'limit' */
    build_spf(limit, &spf);

    /* allocate arrays for squarefree flags and prefix counts */
    squarefree = (bool*)malloc((limit + 1) * sizeof(bool));
    pref       = mem_new(seq_t, (limit + 1));

    squarefree[0] = squarefree[1] = true;   /* 0,1 are treated as squarefree */
    pref[0] = pref[1] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t j = i / p;
        if (j % p == 0)
            squarefree[i] = false;           /* divisible by p^2        */
        else
            squarefree[i] = squarefree[j];   /* inherit property of j */

        pref[i] = pref[i-1] + (squarefree[i] ? 0 : 1);
    }

    max_n_computed = n_req;
}

/* public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* sequence undefined below offset */

    /* need at least n+1 primes */
    init_for_n(n + 1);

    idx_t pa = primes[n - 1];    /* prime(n)   */
    idx_t pb = primes[n];        /* prime(n+1) */

    /* count nonsquarefree integers strictly between pa and pb */
    return pref[pb - 1] - pref[pa];
}
