/*
 * A179996  Composite numbers n such that Sum_{k=0..q} d(k)*10^k is prime
 *          where d(k) are the q+1 distinct prime factors of n, d(0)< d(1) <
 *          ... < d(q).
 * https://oeis.org/A179996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   factor_u64, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation:  Search composite numbers with at least two distinct prime factors.
   For each such number n, compute v = Σ d(k) · 10^k where d(k) are the distinct
   prime factors of n in increasing order.  If v is prime, n is part of the
   sequence.  The function returns the n‑th such composite. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 4;

/* Ensure that the cache contains at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (need <= cache_len)
        return;

    /* Allocate or grow the cache if necessary */
    if (!cache) {
        cache_cap = (need < 16) ? 16 : need;
        cache = mem_new(seq_t, cache_cap);
        cache_len = 0;
    } else if (need > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort(); /* memory exhausted */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Factorization buffers */
    const size_t MAX_FACTS = 64;
    uint64_t primes[MAX_FACTS];
    uint64_t exps[MAX_FACTS];
    size_t cnt;

    /* Generate terms until we have enough */
    while (cache_len < need) {
        uint64_t n = next_candidate++;
        /* Factor n */
        factor_u64(n, primes, exps, &cnt);
        /* Need at least two distinct prime factors */
        if (cnt < 2)
            continue;

        /* Build the concatenated number v = Σ primes[i] * 10^i */
        seq_t v = 0;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t term = seq_mul((seq_t)primes[i], pow10((idx_t)i));
            v += term;
        }

        /* Test primality of v */
        if (is_prime_big(v))
            cache[cache_len++] = (seq_t)n;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1 */
    ensure_cache((size_t)n);
    return cache[(size_t)n - 1];
}
