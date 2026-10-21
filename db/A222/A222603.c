/*
 * A222603  a(1)=1; for n>0, a(n+1) is the least practical number q>a(n) such
 *          that 2(a(n)+1)-q is practical.
 * https://oeis.org/A222603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A222603
 *
 *  Recurrence:
 *      a(1) = 1
 *      a(n+1) = least practical q > a(n) such that 2·(a(n)+1) – q is also practical.
 *
 *  The code keeps a cache of the already computed terms and a list of
 *  primes used for trial division.  Practicality is tested by the
 *  well‑known criterion involving the prime factorisation of the number.
 *
 *  The implementation is generic in the sense that it works for the
 *  arbitrary-precision build (seq_t is a class) as well as for any
 *  fixed-width unsigned integer type.  All allocations for seq_t occur
 *  through the build‑provided helpers seq_alloc and seq_realloc, so
 *  the code does not depend on C++ facilities such as std::vector.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------------ */
/*  Prime table (used only for trial division)                       */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;        /* array of primes (uint64_t)    */
static size_t primes_cnt = 0;          /* number of primes stored       */
static size_t primes_cap = 0;          /* capacity of primes array      */
static uint64_t last_checked = 1;      /* highest integer checked       */

/* ------------------------------------------------------------------ */
/*  Extend the prime list up to limit (inclusive)                     */
/* ------------------------------------------------------------------ */
static void extend_primes_to(uint64_t limit)
{
    if (limit <= last_checked)
        return;

    if (!primes) {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }

    for (uint64_t n = last_checked + 1; n <= limit; ++n) {
        bool prime = true;
        uint64_t sq = (uint64_t)floor_sqrt((seq_t)n);
        for (size_t i = 0; i < primes_cnt && primes[i] <= sq; ++i) {
            if (n % primes[i] == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = n;
        }
    }
    last_checked = limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether n (uint64_t) is a practical number                   */
/* ------------------------------------------------------------------ */
static bool is_practical_uint64(uint64_t n)
{
    if (n == 1)
        return true;                     /* 1 is practical by definition */

    uint64_t remaining = n;
    uint64_t pf[32];
    int exp[32];
    int cnt = 0;

    /* Ensure primes up to sqrt(remaining) are available */
    extend_primes_to((uint64_t)floor_sqrt((seq_t)remaining));

    for (size_t i = 0; i < primes_cnt &&
                       (__uint128_t)primes[i] * primes[i] <= remaining; ++i) {
        uint64_t p = primes[i];
        if (remaining % p != 0)
            continue;
        int e = 0;
        do {
            remaining /= p;
            ++e;
        } while (remaining % p == 0);
        pf[cnt] = p;
        exp[cnt] = e;
        ++cnt;
    }

    if (remaining > 1) {                  /* remaining is a prime factor */
        pf[cnt] = remaining;
        exp[cnt] = 1;
        ++cnt;
    }

    if (cnt == 0 || pf[0] != 2)
        return false;                     /* first prime must be 2 */

    __uint128_t s = 1;                    /* product of σ(p^e) so far  */
    for (int k = 0; k < cnt; ++k) {
        uint64_t p = pf[k];
        int e = exp[k];

        /* Check the practicality condition for this prime */
        if (k > 0 && (__uint128_t)p > s + 1)
            return false;

        /* Compute σ(p^e) = 1 + p + ... + p^e */
        __uint128_t sigma = 1;
        __uint128_t power = 1;
        for (int i = 0; i < e; ++i) {
            power *= p;
            sigma += power;
        }
        s *= sigma;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Cache of already computed sequence values                         */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;            /* array holding sequence values   */
static size_t cache_cap = 0;           /* allocated capacity of cache     */
static size_t cache_sz   = 0;           /* number of values stored         */

/* ------------------------------------------------------------------ */
/*  Grow the cache array to at least 'needed' elements                */
/* ------------------------------------------------------------------ */
static void cache_extend(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Generate sequence values up to index 'n' (1‑based)                 */
/* ------------------------------------------------------------------ */
static void generate_up_to(idx_t n)
{
    if ((idx_t)cache_sz >= n)
        return;                         /* already computed */

    if (cache_sz == 0) {
        cache_extend(1);
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        cache_sz = 1;
    }

    uint64_t prev = (uint64_t)cache[cache_sz - 1];

    for (idx_t idx = (idx_t)(cache_sz + 1); idx <= n; ++idx) {
        uint64_t cand = prev + 1;
        while (1) {
            if (!is_practical_uint64(cand)) {
                ++cand;
                continue;
            }
            uint64_t other = 2 * (prev + 1) - cand;
            if (is_practical_uint64(other))
                break;
            ++cand;
        }
        cache_extend((size_t)idx);
        cache[cache_sz++] = (seq_t)cand;
        prev = cand;
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point: a(n)                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n = 0 */
    generate_up_to(n);
    return cache[n - 1];
}
