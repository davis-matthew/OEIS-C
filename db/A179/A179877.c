/*
 * A179877  Numbers h such that h and h+1 have same contraharmonic mean of the
 *          numbers k < h such that gcd(k, h) = 1 and simultaneously this mean
 *          is an integer (see A179882).
 * https://oeis.org/A179877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Uses a brute‑force search. h is valid if its contraharmonic mean
 *            and that of h+1 are equal and integral
 * Range:     u8 n <= 10, u16 n <= 2281, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ---------- integer square root for uint64_t ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = (uint64_t)sqrt((long double)n);
    while ((r + 1ULL) * (r + 1ULL) <= n) ++r;
    while (r * r > n) --r;
    return r;
}

/* ---------- dynamic list of primes ---------- */
static uint32_t *PRIMES = NULL;
static size_t PRIMES_CNT = 0;
static size_t PRIMES_CAP = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (PRIMES == NULL)
    {
        PRIMES_CAP = 64;
        PRIMES = (uint32_t *)malloc(PRIMES_CAP * sizeof(uint32_t));
    }
    if (PRIMES_CNT == 0)
    {
        PRIMES[0] = 2;
        PRIMES_CNT = 1;
    }
    uint64_t cand = PRIMES_CNT ? PRIMES[PRIMES_CNT - 1] + 1 : 2;
    while (cand <= limit)
    {
        bool prime = true;
        for (size_t i = 0; i < PRIMES_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= cand; ++i)
        {
            if (cand % PRIMES[i] == 0) { prime = false; break; }
        }
        if (prime)
        {
            if (PRIMES_CNT == PRIMES_CAP)
            {
                PRIMES_CAP <<= 1;
                PRIMES = (uint32_t *)realloc(PRIMES, PRIMES_CAP * sizeof(uint32_t));
            }
            PRIMES[PRIMES_CNT++] = (uint32_t)cand;
        }
        ++cand;
    }
}

/* ---------- prime factors of n ---------- */
static void distinct_prime_factors(uint64_t n, uint32_t *pf, size_t *pcnt)
{
    *pcnt = 0;
    uint64_t m = n;
    uint64_t lim = isqrt_u64(m);
    ensure_primes_upto(lim);
    for (size_t i = 0; i < PRIMES_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= m; ++i)
    {
        uint32_t p = PRIMES[i];
        if (m % p == 0)
        {
            pf[(*pcnt)++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) pf[(*pcnt)++] = (uint32_t)m;
}

/* ---------- Euler's totient ---------- */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t result = n;
    uint64_t m = n;
    uint64_t lim = isqrt_u64(m);
    ensure_primes_upto(lim);
    for (size_t i = 0; i < PRIMES_CNT && (uint64_t)PRIMES[i] * PRIMES[i] <= m; ++i)
    {
        uint32_t p = PRIMES[i];
        if (m % p == 0)
        {
            while (m % p == 0) m /= p;
            result = result / p * (p - 1);
        }
    }
    if (m > 1) result = result / m * (m - 1);
    return result;
}

/* ---------- sum of squares of coprime residues ---------- */
static __int128 sum_squares_coprime128(uint64_t n)
{
    if (n <= 1) return 0;
    uint32_t pf[32];
    size_t pcnt;
    distinct_prime_factors(n, pf, &pcnt);
    __int128 total = 0;
    size_t subsets = 1ULL << pcnt;
    for (size_t mask = 0; mask < subsets; ++mask)
    {
        uint64_t d = 1;
        int bits = 0;
        for (size_t i = 0; i < pcnt; ++i)
            if (mask & (1ULL << i))
            {
                d *= pf[i];
                ++bits;
            }
        int mu = (bits % 2 == 0) ? 1 : -1;
        uint64_t M = (n - 1) / d;
        if (M == 0) continue;
        __int128 m = M;
        __int128 sums = m * (m + 1) * (2 * m + 1) / 6;
        __int128 term = (__int128)d * d * sums;
        total += mu * term;
    }
    return total;
}

/* ---------- contraharmonic mean ---------- */
static bool contraharmonic_mean(uint64_t n, seq_t *mean_out)
{
    if (n <= 1) return false;
    uint64_t phi = phi_uint64(n);
    __int128 s1 = (__int128)n * phi / 2;      /* sum of residues */
    if (s1 == 0) return false;
    __int128 s2 = sum_squares_coprime128(n);  /* sum of squares of residues */
    if (s2 % s1 != 0) return false;
    *mean_out = (seq_t)(s2 / s1);
    return true;
}

/* ---------- check h ---------- */
static bool h_is_valid(uint64_t h)
{
    if (h == 1) return true;
    seq_t mean_h, mean_hp1;
    if (!contraharmonic_mean(h, &mean_h)) return false;
    if (!contraharmonic_mean(h + 1, &mean_hp1)) return false;
    return mean_h == mean_hp1;
}

/* ---------- cache for found terms ---------- */
static seq_t *CACHE = NULL;
static size_t CACHE_SZ = 0;
static size_t CACHE_CAP = 0;
static uint64_t LAST_CHECKED = 1;

static void cache_ensure_capacity(size_t need)
{
    if (CACHE_CAP >= need) return;
    size_t new_cap = CACHE_CAP ? CACHE_CAP << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    CACHE = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
    CACHE_CAP = new_cap;
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */

    /* initialise cache on first call */
    if (CACHE == NULL)
    {
        cache_ensure_capacity(64);
        CACHE[0] = 1;          /* a(1) = 1 */
        CACHE_SZ = 1;
        LAST_CHECKED = 1;
    }

    if ((size_t)n <= CACHE_SZ) return CACHE[n - 1];

    /* generate further terms until we have n of them */
    for (uint64_t h = LAST_CHECKED + 1;
         CACHE_SZ < (size_t)n;
         ++h)
    {
        if (h_is_valid(h))
        {
            cache_ensure_capacity(CACHE_SZ + 1);
            CACHE[CACHE_SZ++] = (seq_t)h;
        }
        if (h == UINT64_MAX) break; /* safety */
    }
    LAST_CHECKED = CACHE_SZ ? ((uint64_t)CACHE[CACHE_SZ - 1]) : 1;

    return CACHE[n - 1];
}
