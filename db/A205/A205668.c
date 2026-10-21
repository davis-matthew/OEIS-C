/*
 * A205668  Prime numbers that cannot be expressed as the sum of two lesser
 *          primes of twin prime pairs + 1 or two greater primes of twin prime
 *          pairs - 1.
 * https://oeis.org/A205668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   ensure_sum_cap, ensure_twin_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205668: primes that cannot be expressed as the sum of two
 * lesser primes of twin prime pairs + 1 or two greater primes of twin
 * prime pairs - 1. This implementation generates primes incrementally
 * and keeps the necessary twin‑prime sums in dynamic bitsets. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_sum_cap.h"
#include "../utils/ensure_twin_cap.h"

/* ---------- Miller‑Rabin primality test for 64‑bit numbers ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic bases for 64‑bit numbers */
static const uint64_t mr_bases[] =
{
    2, 325, 9375, 28178, 450775, 9780504, 1795265022
};

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t p[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(p)/sizeof(p[0]); ++i) {
        if (n % p[i] == 0) return n == p[i];
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(mr_bases)/sizeof(mr_bases[0]); ++i) {
        uint64_t a = mr_bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Dynamic arrays for twin primes and sums ---------- */
static uint64_t *twin_small    = NULL; /* lower primes of twin pairs   */
static uint64_t *twin_big      = NULL; /* upper primes of twin pairs   */
static size_t    twin_small_cnt = 0, twin_big_cnt = 0;
static size_t    twin_small_cap = 0, twin_big_cap   = 0;

static uint8_t *sum_small = NULL; /* sums of two tw‑smaller primes +1  */
static uint8_t *sum_big   = NULL; /* sums of two tw‑bigger primes -1   */
static size_t  sum_small_cap = 0, sum_big_cap = 0;

/* ---------- Cache of the sequence ---------- */
static seq_t *cache   = NULL;
static size_t  cache_cnt = 0, cache_cap = 0;

/* ensure cache can hold cnt terms */
static void ensure_cache_cap(size_t cnt)
{
    if (cnt < cache_cap) return;
    size_t new_cap = (cache_cap) ? (cache_cap << 1) : 64;
    if (new_cap < cnt) new_cap = cnt;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* lazily populate until we have at least n terms */
    while (cache_cnt < n) {
        /* ---------- find next prime ---------- */
        static uint64_t cand = 2;
        static uint64_t last_prime = 0;
        uint64_t p;
        /* Handle 2 separately to allow step 2 */
        if (cand == 2) { p = 2; cand = 3; }
        else {
            while (!is_prime_u64(cand)) cand += 2;
            p = cand;
            cand += 2;
        }

        /* ---------- check if p qualifies for the sequence ---------- */
        bool add = false;
        if (p <= 5) {
            add = true;                 /* 2,3,5 are always in the sequence */
        } else {
            bool ok = true;
            /* check p-1 ∈ sum_small ? */
            if (p > 1 && (size_t)(p - 1) < sum_small_cap && sum_small[p - 1]) ok = false;
            /* check p+1 ∈ sum_big ? */
            if (ok && (size_t)(p + 1) < sum_big_cap   && sum_big[p + 1])   ok = false;
            if (ok) add = true;
        }

        if (add) {
            ensure_cache_cap(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)p;
        }

        /* ---------- detect and record twin pairs ---------- */
        if (last_prime != 0 && p - last_prime == 2) {
            uint64_t small = last_prime;
            uint64_t big   = p;
            /* add to twin arrays */
            ensure_twin_cap(&twin_small_cap, &twin_small_cnt, &twin_small);
            ensure_twin_cap(&twin_big_cap,   &twin_big_cnt,   &twin_big);
            twin_small[twin_small_cnt++] = small;
            twin_big[twin_big_cnt++]     = big;

            /* update sums for twin_small (needed for p-1 check) */
            for (size_t i = 0; i < twin_small_cnt; ++i) {
                uint64_t s = twin_small[i] + small;
                ensure_sum_cap(&sum_small, &sum_small_cap, (size_t)s);
                sum_small[s] = 1;
            }
            /* update sums for twin_big (needed for p+1 check) */
            for (size_t i = 0; i < twin_big_cnt; ++i) {
                uint64_t s = twin_big[i] + big;
                ensure_sum_cap(&sum_big, &sum_big_cap, (size_t)s);
                sum_big[s] = 1;
            }
        }

        last_prime = p;
    }

    return cache[n - 1];
}
