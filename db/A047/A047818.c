/*
 * A047818  a(n) is the least number m such that A002313(n)*m - 1 is a square.
 * https://oeis.org/A047818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A047818
 *  a(n) = least m such that A002313(n)*m - 1 is a square.
 *  A002313(n) is the n‑th prime that is either 2 or ≡1 (mod 4).
 *  The function uses a cache of primes and a primitive‑root based
 *  square‑root mod p to compute m for each prime.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t, uint64_t, seq_t); */

/* --------------------------------------------------------------------- */
/*  Prime cache: all primes found so far, and the primes in A002313      */
/* --------------------------------------------------------------------- */
static uint64_t *all_primes   = NULL;
static size_t    all_cnt      = 0;
static size_t    all_cap      = 0;

static uint64_t *target_primes = NULL;
static size_t    target_cnt    = 0;
static size_t    target_cap    = 0;

static uint64_t  last_checked  = 2;     /* last integer examined for primality */

/* Grow a dynamic array to accommodate 'need' elements. */
static void grow_to(size_t *cap, uint64_t **arr, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : *cap << 1;
        while (newcap < need) newcap <<= 1;
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

/* Append a prime to the list of all primes. */
static void add_all_prime(uint64_t p)
{
    grow_to(&all_cap, &all_primes, all_cnt + 1);
    all_primes[all_cnt++] = p;
}

/* Append a prime that belongs to A002313 to its list. */
static void add_target_prime(uint64_t p)
{
    if (p == 2 || (p & 3ULL) == 1) {
        grow_to(&target_cap, &target_primes, target_cnt + 1);
        target_primes[target_cnt++] = p;
    }
}

/* Integer square root for 64‑bit values (no floating point). */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= n / mid) lo = mid;
        else                 hi = mid - 1;
    }
    return lo;
}

/* Simple trial‑division primality test using the cache of primes. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)          return false;
    if (n == 2)         return true;
    if ((n & 1ULL) == 0ULL) return false;

    uint64_t limit = isqrt_uint64(n);
    for (size_t i = 0; i < all_cnt && all_primes[i] <= limit; ++i)
        if (n % all_primes[i] == 0) return false;
    return true;
}

/* Ensure that at least 'n' primes of the A002313 sequence have been found. */
static void ensure_n_target_primes(size_t n)
{
    /* Initialise the prime lists with the first prime 2. */
    if (target_cnt == 0) {
        add_all_prime(2);
        add_target_prime(2);
        last_checked = 2;
    }

    while (target_cnt < n) {
        ++last_checked;
        if (last_checked > 2 && (last_checked & 1ULL) == 0) continue; /* skip even >2 */
        if (is_prime_uint64(last_checked)) {
            add_all_prime(last_checked);
            add_target_prime(last_checked);
        }
    }
}

/* Factor a 64‑bit number into its distinct prime factors. */
static void factor_unique(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t f = 2; f * f <= n; ++f) {
        if (n % f == 0) {
            factors[(*cnt)++] = f;
            while (n % f == 0) n /= f;
        }
    }
    if (n > 1) factors[(*cnt)++] = n;
}

/* Find a primitive root modulo prime p (p > 2). */
static uint64_t primitive_root(uint64_t p)
{
    uint64_t factors[64];
    size_t   fcnt = 0;
    factor_unique(p - 1, factors, &fcnt);

    for (uint64_t g = 2;; ++g) {
        bool ok = true;
        for (size_t i = 0; i < fcnt; ++i) {
            uint64_t exp = (p - 1) / factors[i];
            seq_t  res  = mod_pow((seq_t)g, exp, (seq_t)p);
            if (res == 1) { ok = false; break; }
        }
        if (ok) return g;
    }
}

/* Compute the smallest positive t < p such that t² ≡ –1 (mod p). */
static uint64_t sqrt_minus_one(uint64_t p)
{
    if (p == 2) return 1;      /* Not used for 2 but harmless */

    uint64_t g = primitive_root(p);
    seq_t  tmp = mod_pow((seq_t)g, (p - 1) / 4, (seq_t)p);
    uint64_t t = (uint64_t)tmp;          /* t < p < 2⁶⁴ */
    if (t > p - t) t = p - t;            /* choose the minimal positive solution */
    return t;
}

/* --------------------------------------------------------------------- */
/*  Public function required by the tests.                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* guard against invalid index */

    size_t nn = (size_t)n;
    ensure_n_target_primes(nn);

    uint64_t p = target_primes[nn - 1];   /* zero‑based indexing */

    uint64_t m;
    if (p == 2)
        m = 1;                          /* 2*1-1 = 1² */
    else {
        uint64_t t = sqrt_minus_one(p);
        __uint128_t t2 = (__uint128_t)t * t + 1;  /* t² + 1 */
        m = (uint64_t)(t2 / p);
    }

    return (seq_t)m;                     /* fits in seq_t for any 64‑bit prime */
}
