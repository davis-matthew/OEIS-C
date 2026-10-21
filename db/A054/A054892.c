/*
 * A054892  Smallest prime a(n) such that the sum of n consecutive primes
 *          starting with a(n) is divisible by n.
 * https://oeis.org/A054892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 118, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A054892  */
/*  Smallest prime a(n) such that the sum of n consecutive primes starting with a(n) is divisible by n.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"   /* provided primality test for 64‑bit numbers */

static uint64_t *primes = NULL;      /* list of primes */
static seq_t   *pref   = NULL;      /* prefix sums of primes: pref[i] = sum of primes[0..i-1] */
static size_t   primes_cap = 0;     /* capacity of primes / pref (pref has one extra slot) */
static size_t   primes_cnt = 0;     /* number of primes generated */

static seq_t *cache   = NULL;       /* memoised a(n) values */
static bool *cached   = NULL;       /* flag if cache[n] is valid */
static size_t cache_cap = 0;        /* capacity of cache arrays */

/* -------------------------------------------------------------------------- */
/* Ensure that we have at least `need` primes in `primes`.                     */
/* -------------------------------------------------------------------------- */
static void ensure_primes(size_t need)
{
    if (need <= primes_cnt) return;

    /* first prime */
    if (primes_cnt == 0) {
        /* grow capacity if needed */
        if (primes_cap == 0) {
            primes_cap = 64;
            primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
            pref   = (seq_t*)   malloc((primes_cap + 1) * sizeof(seq_t));
        }
        primes[0] = 2;
        pref[0]   = 0;
        pref[1]   = 2;
        primes_cnt = 1;
    }

    uint64_t cand = primes[primes_cnt - 1] + 1;
    if (cand < 3) cand = 3;
    if ((cand & 1) == 0) cand++;   /* make odd */

    while (primes_cnt < need) {
        /* resize if necessary */
        if (primes_cnt == primes_cap) {
            size_t new_cap = primes_cap * 2;
            uint64_t *newp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
            seq_t   *newq = (seq_t*)   realloc(pref,   (new_cap + 1) * sizeof(seq_t));
            if (!newp || !newq) { /* fallback in low‑memory environments */
                /* terminate silently – caller will retry after smaller allocation */
                return;
            }
            primes = newp; pref = newq;
            primes_cap = new_cap;
        }

        /* test cand for primality */
        if (is_prime_small(cand)) {
            primes[primes_cnt] = cand;
            pref[primes_cnt + 1] = pref[primes_cnt] + (seq_t)cand;
            primes_cnt++;
        }
        cand += 2;   /* skip even numbers */
    }
}

/* -------------------------------------------------------------------------- */
/* Expand cache storage up to at least index `n`.                            */
/* -------------------------------------------------------------------------- */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap?cache_cap*2:n+1;
    if (new_cap < (size_t)(n+1)) new_cap = n+1;
    seq_t *newc = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    bool  *newf = (bool *)realloc(cached, new_cap * sizeof(bool));
    if (!newc || !newf) return;   /* memory error – unlikely in test harness */
    for (size_t i=cache_cap; i<new_cap; ++i) newf[i] = false;
    cache = newc;
    cached = newf;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                      */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset starts at 1 – safety */

    /* return memoised value if available */
    ensure_cache_capacity(n);
    if (cached[(size_t)n]) return cache[(size_t)n];

    idx_t i = 0;            /* start index of potential sequence of n primes */
    while (true) {
        /* ensure we have enough primes to compute the sum */
        ensure_primes((size_t)i + (size_t)n);

        seq_t sum = pref[i + (size_t)n] - pref[i];
        if (sum % (seq_t)n == 0) {
            uint64_t p = primes[i];
            cache[(size_t)n] = (seq_t)p;
            cached[(size_t)n] = true;
            return cache[(size_t)n];
        }
        i++;
    }
}
