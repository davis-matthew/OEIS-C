/*
 * A067467  Primes p such that p-1 has 3 distinct prime factors.
 * https://oeis.org/A067467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 3015, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067467: primes p such that p‑1 has exactly 3 distinct prime factors. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The build system defines seq_t and idx_t.  We only use them implicitly. */

/* ------------------------------------------------------------ */
/*  Sieve data structures and cache.                           */
/* ------------------------------------------------------------ */
static size_t current_limit = 0;            /* sieve upper bound used so far            */
static uint32_t *spf  = NULL;              /* smallest prime factor for every n ≤ limit */
static uint32_t *primes = NULL;            /* list of primes up to the current limit   */
static size_t   primes_cnt = 0;            /* number of primes stored in 'primes'       */

static seq_t *good_primes = NULL;          /* primes for which p‑1 has 3 distinct PF   */
static size_t good_cnt     = 0;            /* number of such primes stored              */

/* ------------------------------------------------------------ */
/* Build a linear sieve up to 'limit'.                         */
/* ------------------------------------------------------------ */
static void build_sieve(size_t limit)
{
    /* allocate / grow the arrays */
    spf    = (uint32_t*)realloc(spf,    (limit + 1) * sizeof(uint32_t));
    primes = (uint32_t*)realloc(primes, (limit + 1) * sizeof(uint32_t));
    if (!spf || !primes) return;            /* allocation failed – caller will handle */

    memset(spf, 0, (limit + 1) * sizeof(uint32_t));
    primes_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = (uint32_t)i;
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt &&
                            primes[j] <= spf[i] &&
                            i * primes[j] <= limit; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
}

/* ------------------------------------------------------------ */
/* Count distinct prime factors of m using the spf array.      */
/* ------------------------------------------------------------ */
static unsigned count_distinct_pf(uint32_t m)
{
    if (m <= 1) return 0;
    unsigned count = 0;
    uint32_t last  = 0;

    while (m > 1) {
        uint32_t p = spf[m];
        if (p != last) {
            ++count;
            last = p;
        }
        while (m % p == 0) m /= p;
    }
    return count;
}

/* ------------------------------------------------------------ */
/* Ensure that at least 'required' good primes are available.  */
/* If the current sieve limit is too small, double it and      */
/* rebuild until enough terms are found.                       */
/* ------------------------------------------------------------ */
static void ensure_sufficient(idx_t required)
{
    if (good_cnt >= required) return;       /* already enough terms */

    if (current_limit == 0) current_limit = 2000000;   /* initial guess */

    while (good_cnt < (size_t)required) {
        /* rebuild the sieve up to the current limit */
        build_sieve(current_limit);

        /* rebuild the list of good primes */
        seq_t *new_good = (seq_t*)realloc(good_primes,
                                         primes_cnt * sizeof(seq_t));
        if (!new_good) return;           /* allocation failure – caller will notice */
        good_primes = new_good;

        good_cnt = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if (count_distinct_pf(p - 1) == 3) {
                good_primes[good_cnt++] = (seq_t)p;
            }
        }

        /* if not enough, increase the limit and try again */
        if (good_cnt < (size_t)required) {
            current_limit *= 2;
        }
    }
}

/* ------------------------------------------------------------ */
/* Main function requested by the build system.                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour for n=0 irrelevant */
    ensure_sufficient(n);
    return good_primes[n - 1];
}
