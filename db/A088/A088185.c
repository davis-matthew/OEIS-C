/*
 * A088185  Numbers which are not the sum of two coprime semiprimes.
 * https://oeis.org/A088185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer not representable as a sum of *
 *            two coprime semiprimes. The routine performs an iterative
 *            search. * It lazily constructs a smallest-prime-factor sieve and
 *            a list of * semiprimes up to the needed range
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_spf.h"

/* --------------------------------------------------------------------- */
/* Static state for the sieve and the semiprime list                      */
/* --------------------------------------------------------------------- */
static bool          initialized    = false;
static idx_t         sieve_limit    = 0;     /* current sieve upper bound   */
static idx_t        *spf            = NULL;  /* smallest prime factor table */
static idx_t        *semis          = NULL;  /* semiprimes ≤ sieve_limit    */
static size_t        semis_cnt      = 0;
static size_t        semis_cap      = 0;

/* --------------------------------------------------------------------- */
/* Grow the sieve until it contains at least 'limit' (inclusive).        */
/* Rebuild the semiprime list after extending.                           */
/* --------------------------------------------------------------------- */
static void ensure_sieve_limit(idx_t limit)
{
    if (limit <= sieve_limit) return;

    idx_t new_limit = sieve_limit ? sieve_limit : 1024;
    while (new_limit < limit) new_limit <<= 1;          /* power‑of‑two growth */

    idx_t *new_spf = NULL;
    build_spf(new_limit, &new_spf);                    /* allocate new SPF   */
    if (spf) free(spf);                                /* free old table     */
    spf = new_spf;
    sieve_limit = new_limit;

    /* rebuild the list of semiprimes up to the new limit */
    semis_cnt = 0;
    for (idx_t i = 4; i <= sieve_limit; ++i) {
        if (spf[i] != i) {                               /* composite          */
            idx_t q = i / spf[i];
            if (spf[q] == q) {                           /* q prime → semiprime */
                if (semis_cnt == semis_cap) {
                    size_t new_cap = semis_cap ? semis_cap * 2 : 128;
                    idx_t *tmp = (idx_t *)realloc(semis, new_cap * sizeof(idx_t));
                    if (!tmp) { free(spf); free(semis); spf = NULL; semis = NULL; return; }
                    semis = tmp;
                    semis_cap = new_cap;
                }
                semis[semis_cnt++] = i;
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Test whether x can be expressed as a sum of two coprime semiprimes.   */
/* --------------------------------------------------------------------- */
static bool can_be_sum_of_coprime_semiprimes(idx_t x)
{
    idx_t half = x / 2;
    for (size_t i = 0; i < semis_cnt && semis[i] <= half; ++i) {
        idx_t s = semis[i];
        idx_t t = x - s;
        if (t < 2) continue;

        /* t must be a semiprime */
        if (spf[t] == t) continue;                      /* t prime          */
        idx_t qt = t / spf[t];
        if (spf[qt] != qt) continue;                    /* t not semiprime  */

        /* factorisation of s */
        idx_t p1 = spf[s];
        idx_t p2 = s / p1;

        /* factorisation of t */
        idx_t p3 = spf[t];
        idx_t p4 = t / p3;

        /* coprime iff all four primes are distinct */
        if (p1 != p3 && p1 != p4 && p2 != p3 && p2 != p4)
            return true;                               /* representation found */
    }
    return false;                                      /* no representation */
}

/* --------------------------------------------------------------------- */
/* Public entry point: compute the nth term of A088185                 */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                      /* offset is 1 */

    if (!initialized) {
        ensure_sieve_limit(1024);                     /* initialise sieve   */
        initialized = true;
    }

    idx_t count = 0;
    idx_t x     = 1;

    for (;; ++x) {
        if (x > sieve_limit)                       /* grow sieve as needed */
            ensure_sieve_limit(x);

        if (!can_be_sum_of_coprime_semiprimes(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
