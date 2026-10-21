/*
 * A050700  Composite numbers n smaller than their decimal reverse but having
 *          the same number of prime factors.
 * https://oeis.org/A050700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 8346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, rev_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050700: Composite numbers n such that n < reverse(n) and ω(n)=ω(reverse(n))
   Search sequentially and memoise the results.
   The first term (offset 1) is 15.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/rev_uint64.h"

#define PRIME_LIMIT   2000000ULL     /* enough for all numbers examined in the tests */
#define MAX_TERMS     200000         /* capacity for memoisation */

/* global data ----------------------------------------------------- */
static idx_t    *primes  = NULL;     /* list of primes up to PRIME_LIMIT   */
static size_t     primes_cnt = 0;    /* number of primes in that list      */
static bool       primes_ok  = false;/* flag: primes array built            */

static seq_t      terms[MAX_TERMS]; /* memoised terms                     */
static size_t     terms_len  = 0;    /* number of terms stored             */
static uint64_t   next_candidate = 2;/* next integer to test               */

/* build prime list once ------------------------------------------ */
static void init_primes(void)
{
    if (primes_ok) return;
    build_primes(PRIME_LIMIT, &primes, &primes_cnt);
    primes_ok = true;
}

/* count prime factors with multiplicity using the prime list  ----- */
static int count_pf(uint64_t x)
{
    if (x < 2) return 0;
    int cnt = 0;
    uint64_t tmp = x;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (p * p > tmp) break;
        while (tmp % p == 0)
        {
            ++cnt;
            tmp /= p;
        }
    }
    if (tmp > 1) ++cnt;        /* remaining prime factor            */
    return cnt;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, handles index 0 gracefully */

    /* initialise primes once */
    init_primes();

    /* generate terms until we have the first n terms */
    while (terms_len < n)
    {
        /* skip numbers < 2 and single‑digit numbers (they cannot satisfy n<rev) */
        while (next_candidate < 2) ++next_candidate;

        uint64_t rev = rev_uint64(next_candidate);

        if (next_candidate < rev)                /* n < rev(n)  */
        {
            /* n must be composite: ω(n) > 1   */
            int cnt_n = count_pf(next_candidate);
            if (cnt_n > 1)                       /* composite */
            {
                int cnt_rev = count_pf(rev);
                if (cnt_n == cnt_rev)            /* same number of prime factors */
                {
                    if (terms_len < MAX_TERMS)
                        terms[terms_len++] = (seq_t)next_candidate;
                    else
                        abort();                  /* buffer overflow – should not happen */
                }
            }
        }
        ++next_candidate;
    }

    return terms[n - 1];
}
