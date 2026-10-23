/*
 * A192226  Numbers k such that all integers in the interval
 *          (k-2*sqrt(sqrt(k)), k] are composite.
 * https://oeis.org/A192226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1996, u32 n <= 6861, u64 n <= 6861,
 *            u128 n <= 6861, big any n
 * Helpers:   _floor_isqrt_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute k such that all integers in (k-2*sqrt(sqrt(k)),k] are composite. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime_big.h"

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
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
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
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
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
