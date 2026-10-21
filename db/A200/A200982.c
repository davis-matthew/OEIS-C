/*
 * A200982  Numbers k such that 1^(lambda(k)/2) + 2^(lambda(k)/2) + ... + (k -
 *          1)^(lambda(k)/2) = 0 (mod k), where lambda is the Carmichael
 *          reduced totient function.
 * https://oeis.org/A200982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k (n≥1) such that sum_{i=1}^{k-1}
 *            i^{λ(k)/2} ≡ 0 (mod k), where λ is Carmichael's reduced totient
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd64, lcm64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"
#include "../utils/lcm64.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static uint64_t next_k = 1;      /* next integer to test for membership */

/* ------------------------------------------------------------------ */
/*  GCD, LCM, and modular exponentiation for 64‑bit unsigned values   */
/* ------------------------------------------------------------------ */

static inline uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) res = (uint64_t)((unsigned __int128)res * base % mod);
        base = (uint64_t)((unsigned __int128)base * base % mod);
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Test whether k satisfies the defining congruence                  */
/* ------------------------------------------------------------------ */
static bool good_k(uint64_t k) {
    if (k == 1) return true;            /* 1 is always in the sequence */

    uint64_t n = k;
    uint64_t lambda = 1;                /* Carmichael lambda of k */
    uint64_t exp2 = 0;                  /* exponent of 2 in k */

    /* factor out all 2's */
    while ((n & 1) == 0) {
        n >>= 1;
        exp2++;
    }

    /* store odd prime factors: up to 20 factors is more than enough */
    struct { uint64_t p; int e; } factors[20];
    int fcnt = 0;

    /* factor the remaining odd part by trial division */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            factors[fcnt].p = p;
            factors[fcnt].e = e;
            fcnt++;

            /* λ(p^e) = (p-1) * p^{e-1} for odd p */
            uint64_t lam = p - 1;
            for (int i = 1; i < e; ++i) lam *= p;
            lambda = lcm64(lambda, lam);
        }
    }
    if (n > 1) {                        /* remaining prime factor */
        uint64_t p = n;
        factors[fcnt].p = p;
        factors[fcnt].e = 1;
        fcnt++;
        uint64_t lam = p - 1;
        lambda = lcm64(lambda, lam);
    }

    /* λ for 2^exp2 */
    uint64_t lam2 = 1;
    if (exp2 == 0) {}
    else if (exp2 == 1) lam2 = 1;
    else if (exp2 == 2) lam2 = 2;
    else lam2 = 1ULL << (exp2 - 2);     /* 2^{exp2-2} for exp2 ≥ 3 */

    lambda = lcm64(lambda, lam2);

    if (lambda & 1) return false;       /* λ must be even */
    uint64_t m = lambda >> 1;           /* λ(k)/2 */

    /* Check condition for every odd prime power */
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = factors[i].p;
        int e = factors[i].e;

        /* φ(p^e) = p^{e-1} * (p-1) for odd p */
        uint64_t phi = 1;
        for (int j = 1; j < e; ++j) phi *= p;
        phi *= (p - 1);

        /* we need m ≡ φ/2 (mod φ) */
        if (m % phi != phi / 2) return false;
    }

    /* Now handle the 2^exp2 part by direct summation */
    if (exp2 > 0) {
        uint64_t mod = 1ULL << exp2;
        uint64_t sum = 0;
        for (uint64_t i = 1; i < mod; ++i) {
            uint64_t term = powmod64(i, m, mod);
            sum += term;
            if (sum >= mod) sum -= mod;
        }
        if (sum != 0) return false;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/*  Ensure dynamic cache has sufficient capacity                        */
/* ------------------------------------------------------------------ */
static inline void ensure_cache(idx_t needed) {
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap * 2 : 64;
        while (newcap < needed) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }
}

/* ------------------------------------------------------------------ */
/*  Build the sequence until we have at least n terms                  */
/* ------------------------------------------------------------------ */
static void build_until(idx_t n) {
    ensure_cache(n);
    while (cache_len < n) {
        if (good_k(next_k)) {
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
}

/*------------------------------------------------------------------*/
/*  Public function: returns a(n)                                   */
/*------------------------------------------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined for offset 1, but harmless */

    build_until(n);
    return cache[n - 1];
}
