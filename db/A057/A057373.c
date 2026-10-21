/*
 * A057373  Numbers k that can be expressed as k = w + x = y*z with w*x = y^2
 *          + z^2 where w, x, y, and z are all positive integers.
 * https://oeis.org/A057373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: k is in A057373 iff there exists a divisor d|k such that
   k² – 4·(d² + (k/d)²) is a perfect square.
   This implementation:
   – Enumerates candidates in increasing order.
   – Skips numbers not divisible by 9 (all known terms are so).
   – For each candidate, factorises with a small prime sieve,
     generates all divisors, and tests the condition using 128‑bit
     arithmetic.
   – Caches found terms so that subsequent non‑mono‑increasing calls
     may reuse previous work. */

/* ------------------------------------------------------------------ */

#include <stdint.h>
#include <stdlib.h>

/* The build supplies typedefs for seq_t and idx_t.  Do not define them. */

/* ------------------------------------------------------------------ */
/*  1.  Prime sieve – enough for factorising candidates up to ≈10¹².  */
/* ------------------------------------------------------------------ */
#define PRIME_SIEVE_LIMIT 2000000          /* > sqrt(10¹²) */
#define MAX_PRIME_COUNT  150000            /* upper estimate for that limit */

static uint32_t prime_list[MAX_PRIME_COUNT];
static size_t   prime_cnt = 0;

/* initialise the prime list once */
static void init_primes(void)
{
    if (prime_cnt != 0) return;                     /* already initialised */

    /* simple sieve of Eratosthenes */
    static unsigned char is_prime[PRIME_SIEVE_LIMIT + 1];
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        is_prime[i] = 1;
    for (size_t p = 2; p * p <= PRIME_SIEVE_LIMIT; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= PRIME_SIEVE_LIMIT; q += p)
                is_prime[q] = 0;

    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            prime_list[prime_cnt++] = (uint32_t)i;
}

/* ------------------------------------------------------------------ */
/*  2.  Factorisation of n (uint64_t)                                 */
/* ------------------------------------------------------------------ */
static void factor_uint64(uint64_t n,
                          uint32_t *pf, int *exp, size_t *len)
{
    *len = 0;
    uint64_t rem = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > rem) break;
        if (rem % p == 0) {
            int cnt = 0;
            while (rem % p == 0) {
                rem /= p;
                ++cnt;
            }
            pf[(*len)] = p;
            exp[(*len)] = cnt;
            ++(*len);
        }
    }
    if (rem > 1) {          /* remaining prime factor */
        pf[(*len)]   = (uint32_t)rem;
        exp[(*len)]  = 1;
        ++(*len);
    }
}

/* ------------------------------------------------------------------ */
/*  3.  Generate all divisors of n                                     */
/* ------------------------------------------------------------------ */
static void generate_divisors(const uint32_t *pf, const int *exp,
                              size_t len, uint64_t *divs, size_t *cnt)
{
    divs[0] = 1;
    *cnt = 1;            /* currently have 1 divisor (1) */

    for (size_t i = 0; i < len; ++i) {
        size_t current_cnt = *cnt;
        uint64_t p = pf[i];
        uint64_t mult = 1;
        for (int e = 1; e <= exp[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < current_cnt; ++j) {
                divs[*cnt] = divs[j] * mult;
                ++(*cnt);
                if (*cnt >= 32768) return;   /* safety overflow guard */
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/*  4.  Integer square root for unsigned __int128                      */
/* ------------------------------------------------------------------ */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64;   /* sqrt(2ⁿ⁸⁻¹) < 2⁶⁴ */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* ------------------------------------------------------------------ */
/*  5.  Test if a candidate k satisfies the defining condition       */
/* ------------------------------------------------------------------ */
static int satisfies_k(uint64_t k)
{
    /* factorise k to obtain its divisors */
    uint32_t pf[64];
    int    exp[64];
    size_t len = 0;
    factor_uint64(k, pf, exp, &len);

    uint64_t divs[32768];
    size_t   div_cnt = 0;
    generate_divisors(pf, exp, len, divs, &div_cnt);

    unsigned __int128 k128 = (unsigned __int128)k;
    unsigned __int128 k2   = k128 * k128;

    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        uint64_t e = k / d;                /* other divisor */

        unsigned __int128 d2 = (unsigned __int128)d * d;
        unsigned __int128 e2 = (unsigned __int128)e * e;
        unsigned __int128 t  = d2 + e2;    /* d² + e² */
        unsigned __int128 four_t = 4 * t;   /* 4*(d² + e²) */

        unsigned __int128 s = k2 - four_t; /* k² – 4*(…) */
        unsigned __int128 sqrt_s = isqrt128(s);
        if (sqrt_s * sqrt_s == s)          /* perfect square? */
            return 1;
    }
    return 0;
}

/* ------------------------------------------------------------------ */
/*  6.  Cached sequence storage                                       */
/* ------------------------------------------------------------------ */
#define INITIAL_CACHE_CAP 64
static seq_t  *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 9;     /* first multiple of 9 to test */

/* enlarge cache if necessary */
static void cache_ensure(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : INITIAL_CACHE_CAP;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return;             /* out of memory – caller will handle */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  7.  Public sequence function                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();          /* guarantee primes are ready */

    /* grow cached terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (satisfies_k(next_k)) {
            cache_ensure();
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k += 9;        /* all known terms are divisible by 9 */
    }

    return cache[(size_t)n - 1];
}
