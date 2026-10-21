/*
 * A065870  n-th prime - n-th semiprime.
 * https://oeis.org/A065870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = nth prime – nth semiprime (OEIS A065870, offset 1)
 *            Implementation uses a sieve for primes and generates all
 *            semiprimes up to the required bound. The routine works for both
 *            C11 and C++17 builds; for an arbitrary‑precision seq_t (C++
 *            only) it keeps data in std::vector to avoid undefined beha...
 * Range:     i8 n <= 26, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/seq_cmp.h"      /* comparator for qsort (C version) */

#if defined(__cplusplus)
#include <vector>
#include <algorithm>
#endif

/* ------------------------------------------------------------ */
/*  Static data for the sieve and the two lists                 */
/* ------------------------------------------------------------ */

static idx_t                 sieve_limit  = 0;          /* current upper limit of sieve */
static unsigned char        *is_comp      = NULL;        /* composite flags */

static idx_t                 semiprime_limit = 0;       /* upper bound of generated semiprimes */

#ifndef __cplusplus
/*  C version – use ordinary malloc / realloc                 */
static idx_t                *primes        = NULL;       /* array of primes            */
static size_t                primes_cnt    = 0;          /* number of primes stored    */
static size_t                primes_cap    = 0;          /* capacity of the primes     */

static seq_t                *semiprimes    = NULL;       /* array of semiprimes        */
static size_t                semi_cnt      = 0;          /* number of semiprimes       */
static size_t                semi_cap      = 0;          /* capacity of the semiprimes */
#else
/*  C++ version – use vectors which handle constructors       */
static std::vector<idx_t>    primes;                /* prime list                 */
static std::vector<seq_t>    semiprimes;            /* semiprime list             */
#endif

/* ------------------------------------------------------------ */
/*  Helper: extend sieve until we have at least `n` primes and  */
/*  `n` semiprimes.                                            */
/* ------------------------------------------------------------ */
static void ensure_primes_and_semiprimes(idx_t n)
{
    if (n < 1) return;                     /* nothing required */

#if defined(__cplusplus)
    /* -------- C++ implementation: use vectors ---------------- */
    for (;;)
    {
        /* Grow the sieve if needed */
        if (sieve_limit < n * 20) {        /* simple heuristic for enough numbers */
            /* Rough upper bound for nth prime: n(log n + log log n) + 10 */
            double d = (double)n;
            idx_t  lim = (n < 6) ? 20 :
                         (idx_t)(d * (log(d) + log(log(d))) + 10.0);
            if (lim <= sieve_limit) lim = sieve_limit + 1;

            primes.clear();
            is_comp = (unsigned char *)realloc(is_comp,
                                                (size_t)lim + 1);
            if (!is_comp) abort();
            memset(is_comp, 0, (size_t)lim + 1);

            for (idx_t i = 2; i <= lim; ++i)
                if (!is_comp[i]) {
                    primes.push_back(i);
                    if (i * i <= lim)
                        for (idx_t j = i * i; j <= lim; j += i)
                            is_comp[j] = 1;
                }
            sieve_limit = lim;
        }

        /* Generate semiprimes <= sieve_limit */
        semiprimes.clear();
        for (size_t i = 0; i < primes.size(); ++i) {
            idx_t pi = primes[i];
            for (size_t j = i; j < primes.size(); ++j) {
                idx_t pj = primes[j];
                idx_t prod = pi * pj;
                if (prod > sieve_limit) break;
                semiprimes.push_back((seq_t)prod);
            }
        }
        std::sort(semiprimes.begin(), semiprimes.end());

        if (primes.size() >= (size_t)n && semiprimes.size() >= (size_t)n)
            return;   /* sufficient data */

        /* Double the limit and try again */
        sieve_limit *= 2;
    }
#else
    /* -------- C implementation: use malloc / realloc --------- */
    for (;;) {
        /* Ensure we have at least `n` primes.  Estimate upper bound for nth prime. */
        double dn = (double)n;
        idx_t   limit;
        if (n < 6)
            limit = 20;
        else {
            double est = dn * (log(dn) + log(log(dn)));
            limit = (idx_t)(est + 10.0);   /* safety margin */
        }

        /* Grow sieve if necessary */
        if (limit > sieve_limit) {
            free(is_comp);
            free(primes);
            is_comp  = (unsigned char *)calloc((size_t)limit + 1,
                                                sizeof(unsigned char));
            if (!is_comp) abort();

            primes_cap = (size_t)(limit / log((double)limit)) * 2 + 10;
            primes     = (idx_t *)malloc(primes_cap * sizeof(idx_t));
            if (!primes) abort();

            primes_cnt = 0;
            for (idx_t i = 2; i <= limit; ++i) {
                if (!is_comp[i]) {
                    if (primes_cnt == primes_cap) {
                        primes_cap = primes_cap * 2 + 10;
                        primes = (idx_t *)realloc(primes,
                                                 primes_cap * sizeof(idx_t));
                        if (!primes) abort();
                    }
                    primes[primes_cnt++] = i;
                    if (i * i <= limit) {
                        for (idx_t j = i * i; j <= limit; j += i)
                            is_comp[j] = 1;
                    }
                }
            }
            sieve_limit = limit;
        }

        /* Generate all semiprimes <= sieve_limit */
        free(semiprimes);
        semiprimes = NULL;
        semi_cnt   = 0;
        semi_cap   = 0;

        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t pi = primes[i];
            for (size_t j = i; j < primes_cnt; ++j) {
                idx_t pj = primes[j];
                idx_t prod = pi * pj;
                if (prod > sieve_limit) break;
                if (semi_cnt == semi_cap) {
                    semi_cap = semi_cap * 2 + 10;
                    semiprimes = (seq_t *)realloc(semiprimes,
                                                 semi_cap * sizeof(seq_t));
                    if (!semiprimes) abort();
                }
                semiprimes[semi_cnt++] = (seq_t)prod;
            }
        }

        qsort(semiprimes, semi_cnt, sizeof(seq_t), seq_cmp);

        if (primes_cnt >= (size_t)n && semi_cnt >= (size_t)n)
            return;   /* done */

        /* Increase bound and repeat */
        limit *= 2;
    }
#endif
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n==0 – return 0 safely */

    ensure_primes_and_semiprimes(n);

#if defined(__cplusplus)
    /* C++ path – vectors hold the data */
    seq_t p = (seq_t)primes[n - 1];
    seq_t s = semiprimes[n - 1];
    return p - s;
#else
    /* C path – arrays */
    seq_t p = (seq_t)primes[n - 1];
    seq_t s = semiprimes[n - 1];
    return p - s;
#endif
}
