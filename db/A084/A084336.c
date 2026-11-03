/*
 * A084336  a(n) is the smallest composite number not already in the sequence
 *          such that the absolute values of the successive differences are
 *          distinct primes.
 * https://oeis.org/A084336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A084336: a(n) is the smallest composite number not already in the
 * sequence such that |a(k+1) - a(k)| are distinct primes.
 * Starting value a(0) = 4.
 * The implementation memoises all previously computed terms and uses a
 * table of primes to generate the next term.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
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

/* helper prototypes */
static inline bool  used_in_cache(seq_t x);
static inline bool  is_composite(seq_t x);

/* static storage for memoisation and prime data */
static seq_t  *cache      = NULL;   /* generated sequence */
static size_t cache_len  = 0;       /* number of stored terms */
static size_t cache_cap  = 0;       /* allocated capacity */

static uint64_t *primes    = NULL;  /* list of primes up to LIMIT */
static bool     *prime_used = NULL;/* which primes already used as differences */
static size_t   primes_cnt = 0;     /* number of primes */

/* initialise static data on first use */
static void init(void)
{
    const idx_t LIMIT = 200000;          /* enough for ≈10 000 terms */
    build_primes_upto(LIMIT, &primes, &primes_cnt);

    prime_used = (bool*)calloc(primes_cnt, sizeof(bool));

    cache_cap = 128;                     /* initial capacity */
    cache     = mem_new(seq_t, cache_cap);
}

/* check whether a value is already in the memoised sequence */
static inline bool used_in_cache(seq_t x)
{
    for (size_t i = 0; i < cache_len; ++i)
        if (cache[i] == x) return true;
    return false;
}

/* test whether a number is composite (>=4 and not prime) */
static inline bool is_composite(seq_t x)
{
    if (x < (seq_t)4) return false;
    return !is_prime_big(x);
}

/* public entry point */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) { init(); initialized = true; }

    /* generate terms up to index n if required */
    while (cache_len <= (size_t)n) {
        /* set first element */
        if (cache_len == 0) {
            cache[0] = (seq_t)4;
            cache_len = 1;
            continue;
        }

        seq_t last = cache[cache_len - 1];
        seq_t best_candidate = 0;
        bool  best_set = false;
        size_t best_prime_idx = SIZE_MAX;

        /* inspect all unused primes */
        for (size_t i = 0; i < primes_cnt; ++i) {
            if (prime_used[i]) continue;

            seq_t p = (seq_t)primes[i];

            /* candidate L + p */
            seq_t cand1 = last + p;
            if (!used_in_cache(cand1) && is_composite(cand1)) {
                if (!best_set || cand1 < best_candidate) {
                    best_candidate = cand1;
                    best_prime_idx = i;
                    best_set = true;
                }
            }

            /* candidate L - p (if positive) */
            if (last > p) {
                seq_t cand2 = last - p;
                if (!used_in_cache(cand2) && is_composite(cand2)) {
                    if (!best_set || cand2 < best_candidate) {
                        best_candidate = cand2;
                        best_prime_idx = i;
                        best_set = true;
                    }
                }
            }

            /* early exit optimization */
            if (best_set && p > last && (last + p) >= best_candidate)
                break;
        }

        if (!best_set) abort(); /* should never happen */

        /* grow cache if necessary */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len] = best_candidate;
        prime_used[best_prime_idx] = true;
        ++cache_len;
    }

    return cache[(size_t)n];
}
