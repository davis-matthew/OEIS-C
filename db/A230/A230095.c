/*
 * A230095  Odious numbers (A000069) that are the product of exactly two
 *          distinct primes.
 * https://oeis.org/A230095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth odious integer that is a product of exactly two
 *            distinct primes. The implementation iterates over natural
 *            numbers, tests odiousness (odd popcount) and semiprimality with
 *            distinct prime factors, caching found values for repeated calls
 * Range:     u8 n <= 38, u16 n <= 8186, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- integer square root (64‑bit) ------------------------------ */
static uint64_t isqrt_uint64(uint64_t v)
{
    if (v < 2) return v;
    uint64_t lo = 1, hi = v >> 1;            /* hi = v/2 is safe */
    while (lo <= hi)
    {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                                /* hi is floor(sqrt(v)) */
}

/* ---------- odious test ---------------------------------------------- */
static bool is_odious_uint64(uint64_t x)
{
    /* the number of set bits is odd → odious */
    bool parity = false;
    while (x)
    {
        parity = !parity;
        x &= x - 1;                           /* drop lowest set bit */
    }
    return parity;
}

/* ---------- prime list generation ------------------------------------ */
static uint64_t *primes = NULL;      /* dynamic array of primes          */
static size_t  primes_cnt  = 0;      /* number of stored primes           */
static size_t  primes_cap  = 0;      /* allocated capacity                */
static uint64_t primes_limit = 0;    /* largest prime stored              */

static void grow_primes_up_to(uint64_t limit)
{
    if (limit <= primes_limit) return;

    if (primes_cnt == 0)
    {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes[0] = 2;
        primes_cnt = 1;
        primes_limit = 2;
    }

    for (uint64_t cand = primes_limit + 1; cand <= limit; ++cand)
    {
        bool prime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p > cand / p) break;
            if (cand % p == 0) { prime = false; break; }
        }
        if (prime)
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap <<= 1;
                primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = cand;
            primes_limit = cand;
        }
    }
}

/* ---------- distinct‑semiprime test ---------------------------------- */
static bool is_semiprime_distinct(uint64_t n)
{
    if (n < 6) return false;                /* 2 * 3 is the smallest */

    uint64_t p = 0;
    uint64_t sq = isqrt_uint64(n);
    grow_primes_up_to(sq);
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t prime = primes[i];
        if (prime > n / prime) break;
        if (n % prime == 0) { p = prime; break; }
    }
    if (!p) return false;                  /* n is prime or 1 */

    uint64_t q = n / p;
    if (q == p) return false;              /* repeated factor */

    /* check that q is prime */
    sq = isqrt_uint64(q);
    grow_primes_up_to(sq);
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint64_t prime = primes[i];
        if (prime > q / prime) break;
        if (q % prime == 0) return false;
    }
    return true;
}

/* ---------- sequence cache ------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t cur = 0;                 /* last integer examined */

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    cache_cap = cache_cap ? cache_cap * 2 : 16;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* ---------- public sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for 0, be tolerant */

    while (cache_len < (size_t)n)
    {
        ++cur;
        if (!is_odious_uint64(cur)) continue;
        if (!is_semiprime_distinct(cur)) continue;
        grow_cache(cache_len + 1);
        cache[cache_len++] = (seq_t)cur;
    }
    return cache[n - 1];
}
