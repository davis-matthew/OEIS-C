/*
 * A051227  Numbers m such that the Bernoulli number B_{2*m} has denominator
 *          42.
 * https://oeis.org/A051227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A051227: numbers m such that the Bernoulli number B_{2*m}
 *  has denominator 42.  Using von Staudt–Clausen:
 *      denom(B_{2m}) = Π p where (p−1) | 2m.
 *  We need all such primes to be exactly {2,3,7}.  Thus m must be odd, divisible
 *  by 3, and must not cause any other prime p>7 with (p−1) | 2m.  The
 *  function below generates the sequence on demand and caches the results.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- Miller–Rabin primality test for 64‑bit numbers --------- */
static bool mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod, __uint128_t *res)
{
    *res = (__uint128_t)a * b % mod;
    return true;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t power  = base % mod;
    while (exp) {
        if (exp & 1)
            result = (result * power) % mod;
        power = (power * power) % mod;
        exp >>= 1;
    }
    return (uint64_t)result;
}

static bool isPrimeU64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smallPrimes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                           19ULL,23ULL,29ULL,31ULL,37ULL,41ULL};
    for (size_t i = 0; i < sizeof(smallPrimes)/sizeof(smallPrimes[0]); ++i) {
        uint64_t p = smallPrimes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for all 64‑bit numbers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_pow_uint64(x, 2, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --------- Test whether a given m satisfies the condition --------- */
static bool test_m(uint64_t m)
{
    /* m must be odd and divisible by 3 */
    if ((m & 1ULL) == 0ULL) return false;
    if (m % 3ULL != 0ULL) return false;

    uint64_t G = 2ULL * m;          /* 2*m : product of all primes with (p-1)|2m */

    /* factor G */
    uint64_t fac[64];
    uint8_t  exp[64];
    size_t   cnt = 0;
    uint64_t x = G;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            fac[cnt] = p;
            exp[cnt] = 0;
            while (x % p == 0) {
                x /= p;
                ++exp[cnt];
            }
            ++cnt;
        }
    }
    if (x > 1) {
        fac[cnt] = x;
        exp[cnt] = 1;
        ++cnt;
    }

    /* enumerate all divisors of G */
    uint64_t divs[256];
    int divcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = fac[i];
        uint8_t  e = exp[i];
        int oldcnt = divcnt;
        uint64_t cur = 1;
        for (uint8_t k = 1; k <= e; ++k) {
            cur *= p;
            for (int j = 0; j < oldcnt; ++j) {
                divs[divcnt++] = divs[j] * cur;
            }
        }
    }

    /* check that the only primes p with p-1 | 2m are 2,3,7 */
    for (int i = 0; i < divcnt; ++i) {
        uint64_t d = divs[i];
        uint64_t p = d + 1ULL;     /* candidate prime */
        if (p == 2ULL || p == 3ULL || p == 7ULL) continue;
        if (isPrimeU64(p)) return false; /* extra prime found */
    }
    return true;
}

/* --------- Sequence builder / cache --------- */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static uint64_t next_candidate = 3ULL;

/* extend the cache up to at least target terms */
static void extend_cache(idx_t target)
{
    while (cache_size < target) {
        if (test_m(next_candidate)) {
            seq_t *tmp = (seq_t*)realloc(cache, (cache_size + 1) * sizeof(seq_t));
            if (!tmp) { free(cache); abort(); }
            cache = tmp;
            cache[cache_size++] = (seq_t)next_candidate;
        }
        next_candidate += 6ULL; /* next odd multiple of 3 */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 */
    if (cache_size < n) extend_cache(n);
    return cache[n - 1];
}
