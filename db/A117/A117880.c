/*
 * A117880  a(1) = 4; a(n) is smallest semiprime > 2*a(n-1).
 * https://oeis.org/A117880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A117880 – Smallest semiprime > 2*a(n‑1)
 * a(1) = 4
 * a(n) = min{ s ∈ S : s > 2*a(n‑1) }   where S = {..., pq | p,q primes}
 *
 * The implementation stores computed terms in a static cache and
 * grows it as needed.  To test whether a candidate is a semiprime
 * we try to divide it by a precomputed list of primes up to 1 000 000.
 * The first divisor found is used to check whether the co‑factor is
 * also prime (using the library routine is_prime_big).  This is
 * sufficient for all terms that fit within the tested range.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------------------------------------------------ */
/*  Helper: maintain a static list of primes up to a dynamic limit   */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;
static size_t  primes_cnt = 0;
static uint64_t primes_limit = 0;
#define INIT_PRIME_LIMIT 1000000

static void ensure_primes_up_to(uint64_t limit)
{
    if (primes_limit >= limit)
        return;
    /* free old list and build a new one */
    if (primes != NULL)
        free(primes);
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Test whether x is a semiprime (product of exactly two primes)     */
/* ------------------------------------------------------------------ */
static bool is_semiprime(seq_t x)
{
    if (x < (seq_t)4)          /* the smallest semiprime is 4 */
        return false;

    seq_t sqrtx = floor_sqrt(x);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t   ps = (seq_t)p;
        if (ps > sqrtx)
            break;
        if ((x % ps) == (seq_t)0) {
            seq_t q = x / ps;
            if (q == ps)            /* perfect square of a prime */
                return true;
            return is_prime_big(q);  /* check if co‑factor is prime */
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache: index 1 maps to a(1) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_up_to = 0;

    if (n == 0)
        return (seq_t)0;      /* undefined, but safe */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 64;  /* start with some room */
        cache = mem_new(seq_t, (cache_cap + 1));
        cache[0] = (seq_t)0;      /* unused */
        cache[1] = (seq_t)4;      /* a(1) = 4 */
        cache_up_to = 1;
        ensure_primes_up_to(INIT_PRIME_LIMIT);
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        cache = mem_resize(seq_t, cache, (new_cap + 1));
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cand = prev * (seq_t)2 + (seq_t)1;   /* start just above 2*prev */

        while (!is_semiprime(cand)) {
            cand += (seq_t)1;
        }
        cache[i] = cand;
    }
    cache_up_to = (size_t)n;

    return cache[n];
}
