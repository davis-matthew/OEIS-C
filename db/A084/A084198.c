/*
 * A084198  Primes p such that there exists exactly one prime q<p with q+1
 *          dividing p+1.
 * https://oeis.org/A084198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A084198: primes p such that there exists exactly one prime
    q<p with (q+1) | (p+1).  The function searches primes sequentially,
    checks the condition by factorising p+1, and memoises the approved
    primes.  The sequence is defined by a search, not by a closed
    formula.                                       */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/build_primes_upto.h"

/* ---------- static state (memoisation and prime sieve) ---------- */

static seq_t  *cache      = NULL;   /* cached sequence values      */
static size_t  cache_len  = 0;      /* number of cached terms      */
static size_t  cache_cap  = 0;      /* allocated capacity          */
static uint64_t last_checked = 1;   /* last integer examined      */

/* prime list used for factorisation */
static uint64_t *primes   = NULL;   /* array of primes             */
static size_t   primes_cnt = 0;     /* number of primes            */
static uint64_t primes_limit = 0;   /* maximum sieve limit         */

/* ---------- helpers ---------- */

/* Integer square‑root (floor) ----------------------------------- */
static uint64_t
isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

/* Ensure we have primes up to at least 'limit' (inclusive). ------ */
static void
ensure_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;
    /* free old array if any */
    free(primes);
    /* build new prime list */
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* Count eligible prime q for a candidate prime p.  Return true if
   exactly one such q exists. ------------------------------------ */
static bool
has_exactly_one_q_prime(uint64_t p)
{
    uint64_t d = p + 1;
    uint64_t tmp = d;

    /* factor d using the prime list */
    uint64_t facs[64];
    unsigned int exps[64];
    size_t nf = 0;

    uint64_t sqrt_d = isqrt_u64(tmp);
    ensure_primes_up_to(sqrt_d + 1);   /* guarantee primes for factorisation */

    for (size_t i = 0; i < primes_cnt && primes[i] <= sqrt_d; ++i) {
        uint64_t q = primes[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            unsigned int e = 0;
            while (tmp % q == 0) {
                tmp /= q;
                ++e;
            }
            facs[nf] = q;
            exps[nf] = e;
            ++nf;
            sqrt_d = isqrt_u64(tmp);
        }
    }
    if (tmp > 1) {   /* remaining prime factor > sqrt(d) */
        facs[nf] = tmp;
        exps[nf] = 1;
        ++nf;
    }

    /* enumerate all divisors of d */
    uint64_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < nf; ++i) {
        uint64_t f = facs[i];
        unsigned int e = exps[i];
        size_t old_cnt = div_cnt;
        uint64_t pow = 1;
        for (unsigned int k = 1; k <= e; ++k) {
            pow *= f;
            for (size_t j = 0; j < old_cnt; ++j)
                divs[div_cnt++] = divs[j] * pow;
        }
    }

    /* count primes q such that q+1 | d and q < p */
    int cnt = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t r = divs[i];
        if (r <= 1) continue;
        uint64_t q = r - 1;
        if (q >= 2 && q < p && is_prime(q)) {
            if (++cnt > 1) return false;
        }
    }
    return cnt == 1;
}

/* Append a newly found sequence term to the cache. -------------- */
static void
append_cache(uint64_t p)
{
    if (cache_len >= cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = (seq_t)p;
}

/* ---------- public sequence function ---------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* return cached term when available */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* initialise prime list if needed */
    if (primes == NULL) ensure_primes_up_to(100);   /* a small start */

    /* search for further terms until we have enough */
    for (uint64_t cand = last_checked + 1; ; ++cand) {
        if (!is_prime(cand)) continue;

        if (has_exactly_one_q_prime(cand))
            append_cache(cand);

        if (cache_len >= (size_t)n) {
            last_checked = cand;
            return cache[n - 1];
        }
        last_checked = cand;
    }
}
