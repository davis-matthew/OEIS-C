/*
 * A025307  Numbers that are the sum of 2 distinct nonzero squares in exactly
 *          6 ways.
 * https://oeis.org/A025307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A025307 - Numbers that are the sum of 2 distinct *
 *            nonzero squares in exactly 6 ways. The implementation scans
 *            natural * numbers, counts the unordered distinct representations
 *            using prime * factorisation, and collects the first n terms on
 *            demand
 * Range:     u8 none, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- helpers ---------- */

/* integer square root for 64‑bit unsigned values */
static inline uint32_t floor_sqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;          /* largest power of 4 ≤ 2⁶⁴ */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (uint32_t)r;
}

/* binary search for a square number */
static inline bool is_square(idx_t x, uint32_t *sqs, uint32_t sqc)
{
    uint32_t lo = 0, hi = sqc - 1;
    while (lo <= hi) {
        uint32_t mid = (lo + hi) >> 1;
        seq_t v = (seq_t)sqs[mid];
        if (v == x) return true;
        if (v < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return false;
}

/* ---------- global state ---------- */

static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t   spf_limit = 0;        /* highest number spf covers */

static uint32_t *sqs = NULL;          /* precomputed squares */
static uint32_t sqc   = 0;            /* number of squares in sqs */

static seq_t  *cache = NULL;          /* collected terms of the sequence */
static size_t  cache_len  = 0, cache_cap = 0;

static idx_t   last_checked = 0;      /* last integer that was processed */
static idx_t   max_limit    = 0;       /* current sieve/square limit */


/* build/extend the SPF table up to 'limit' */
static void ensure_spf(uint64_t limit)
{
    if (spf_limit >= limit) return;

    spf = (uint32_t *)realloc(spf, (limit + 1) * sizeof(uint32_t));
    for (size_t i = spf_limit + 1; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (spf[i] == 0) { /* i is prime */
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    for (size_t i = spf_limit + 1; i <= limit; ++i)
        if (spf[i] == 0) spf[i] = (uint32_t)i;

    spf_limit = limit;
}

/* build/extend square table up to 'limit' */
static void ensure_squares(uint64_t limit)
{
    uint32_t new_sqc = floor_sqrt_u64(limit) + 1;
    if (new_sqc <= sqc) return;           /* already enough */

    sqs = (uint32_t *)realloc(sqs, new_sqc * sizeof(uint32_t));
    for (uint32_t i = sqc; i < new_sqc; ++i)
        sqs[i] = i * i;
    sqc = new_sqc;
}

/* add a new term to the cache, growing if necessary */
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1 but guard anyway */

    /* initialise tables on first use */
    if (max_limit == 0) {
        if (ensure_spf) {} /* silence unused warning */
        uint64_t init_limit = 1000000;          /* start guess */
        ensure_spf(init_limit);
        ensure_squares(init_limit);
        max_limit = init_limit;
    }

    /* collect terms until we have at least n of them */
    while (cache_len < n) {
        /* If we have reached the current limit, extend it */
        if (last_checked >= max_limit) {
            uint64_t new_limit = max_limit * 2 + 1000;
            ensure_spf(new_limit);
            ensure_squares(new_limit);
            max_limit = new_limit;
            continue;
        }

        /* process the next integer */
        ++last_checked;
        idx_t x = last_checked;

        /* prime factorisation using SPF */
        idx_t m = x;
        bool representable = true;
        uint64_t prod = 1;            /* product of (e+1) for primes 1 mod 4 */

        while (m > 1) {
            uint32_t p = spf[m];
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p % 4 == 3) {
                if (e & 1) {                 /* odd exponent of 3 mod 4 prime */
                    representable = false;
                    break;
                }
            } else if (p % 4 == 1) {
                prod *= (e + 1);
                if (prod > 16) {             /* r2 would exceed 64 -> >6 pairs */
                    representable = false;
                    break;
                }
            }                               /* p == 2 is ignored */
        }

        if (representable) {
            uint64_t r2 = 4 * prod;          /* total ordered integer reps */

            /* subtract contributions from squares and twice‑squares */
            uint64_t sub = 0;
            if (is_square(x, sqs, sqc)) sub += 4;
            if ((x & 1u) == 0 && is_square(x >> 1, sqs, sqc)) sub += 4;

            if (r2 >= sub && ((r2 - sub) % 8) == 0) {
                uint64_t ways = (r2 - sub) / 8;   /* unordered distinct pairs */
                if (ways == 6) cache_add(x);
            }
        }
    }

    return cache[n - 1];
}
