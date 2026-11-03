/*
 * A192226  Numbers k such that all integers in the interval
 *          (k-2*sqrt(sqrt(k)), k] are composite.
 * https://oeis.org/A192226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 2199, u32 n <= 14432, u64 n <= 14432,
 *            u128 n <= 14432, big any n
 * Validated: standard, pass, 68 terms through n = 6861 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute k such that all integers in (k-2*sqrt(sqrt(k)),k] are composite. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Binary search avoiding overflow */
static inline idx_t _floor_isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t lo = 1, hi = n >> 1;            /* hi < sqrt(max) */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid > n / mid)                /* mid*mid > n   -> overflow or too big */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

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

/* ---------------- Small prime sieve ---------------- */
static bool sieve_inited = false;
static uint8_t *sieve_bits = NULL;
static idx_t sieve_limit = 0;
static size_t sieve_bytes = 0;

static void init_sieve(idx_t limit)
{
    sieve_limit = limit;
    /* use 1 bit per odd number: the bit is 0 → prime, 1 → composite */
    idx_t bits_needed = (limit >> 1) + 1;          /* odds only */
    sieve_bytes   = (bits_needed + 7) >> 3;
    sieve_bits    = (uint8_t*)malloc(sieve_bytes);
    if (!sieve_bits) exit(1);
    memset(sieve_bits, 0, sieve_bytes);           /* 0 == prime for odds */

    for (idx_t p = 3; p * p <= limit; p += 2) {
        idx_t shift = (p - 3) >> 1;
        if (sieve_bits[shift >> 3] & (1U << (shift & 7))) continue;  /* composite */
        for (idx_t j = p * p; j <= limit; j += p << 1) {
            sieve_bits[(j - 3) >> 4] |= (1U << (((j - 3) >> 1) & 7));
        }
    }
}

static inline bool is_prime_small(idx_t n)
{
    if (n < 2) return false;
    if ((n & 1ULL) == 0) return n == 2;
    if (n > sieve_limit) return false;   /* will be checked elsewhere */
    idx_t shift = (n - 3) >> 1;
    return !(sieve_bits[shift >> 3] & (1U << (shift & 7)));
}

/* ----------------- Sequence ----------------- */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static bool cache_inited   = false;
    static seq_t *cache        = NULL;
    static idx_t cache_len     = 0;
    static size_t cache_cap    = 0;

    if (!cache_inited) {
        /* a rough upper bound for the sieve: 300 M is enough for first 10 k terms */
        init_sieve(300000000ULL);

        cache_cap = 64;
        cache     = mem_new(seq_t, cache_cap);
        if (!cache) exit(1);
        cache[0]  = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
        cache_inited = true;
    }

    /* return earlier computed value if available */
    if (cache_len >= n) return cache[n-1];

    /* generate terms up to index n */
    while (cache_len < n) {
        seq_t cand = cache[cache_len-1] + 1;  /* next integer after last term */

        for (;;) {
            /* skip values less than 3; they are not composite */
            if (cand <= (seq_t)2) { cand++; continue; }

            /* is cand composite? */
            bool cand_is_prime;
            if (cand <= (seq_t)sieve_limit)
                cand_is_prime = is_prime_small((idx_t)cand);
            else
                cand_is_prime = is_prime_big(cand);   /* Miller‑Rabin */
            if (cand_is_prime) { cand++; continue; }

            /* compute window length L = floor(2*sqrt(sqrt(k))) */
            idx_t k64 = (idx_t)cand;
            idx_t r   = _floor_isqrt_u64(k64);          /* floor(sqrt(k)) */
            idx_t u   = _floor_isqrt_u64(r);           /* floor(sqrt(sqrt(k))) */
            seq_t win_len = (seq_t)(2 * u);            /* base length 2*u */

            /* determine if we must add 1 */
            {
                idx_t inc = 2*u + 1;                  /* candidate extra part */
                /* left = 16 * k ; right = inc^4 */
                unsigned __int128 left = (unsigned __int128)16 * k64;
                unsigned __int128 inc2 = (unsigned __int128)inc * inc;    /* inc^2 */
                unsigned __int128 right = inc2 * inc2;                   /* inc^4 */
                if (left >= right) win_len = (seq_t)(win_len + 1);
            }

            /* start of interval: all integers > k - win_len */
            seq_t start = cand - win_len;
            if (start < (seq_t)2) start = (seq_t)2;

            /* check every integer in [start, cand] for primality */
            bool ok = true;
            for (seq_t m = start; m <= cand; ++m) {
                bool m_prime;
                if (m <= (seq_t)sieve_limit)
                    m_prime = is_prime_small((idx_t)m);
                else
                    m_prime = is_prime_big(m);
                if (m_prime) { ok = false; break; }
            }

            if (ok) {                      /* found the next term */
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    cache = mem_resize(seq_t, cache, cache_cap);
                    if (!cache) exit(1);
                }
                cache[cache_len++] = cand;
                break;
            }
            cand++;                         /* try next candidate */
        }
    }
    return cache[n-1];
}
