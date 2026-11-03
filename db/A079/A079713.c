/*
 * A079713  Numbers k such that bigomega(sigma(k)) = sigma(bigomega(k)).
 * https://oeis.org/A079713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 875 (2026-11-03)
 * Helpers:   bigomega, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that bigomega(sigma(k)) = sigma(bigomega(k)) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/mem.h"

/*----------- helper to compute σ(n) using trial division -----------*/
static idx_t sigma_idx(idx_t n) {
    if (n <= 1) return 1;
    idx_t result = 1;
    idx_t m = n;
    for (idx_t p = 2; (uint64_t)p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            idx_t pe = 1;
            while (m % p == 0) {
                m /= p;
                ++e;
                pe *= p;
            }
            /* term = (p^(e+1) - 1) / (p - 1) */
            __uint128_t numerator = (__uint128_t)pe * p - 1;   /* p^(e+1)-1 */
            idx_t denom = p - 1;
            idx_t term = (idx_t)(numerator / denom);
            result = (idx_t)((__uint128_t)result * term);
        }
    }
    if (m > 1) {
        /* remaining prime factor */
        result = (idx_t)((__uint128_t)result * (m + 1));
    }
    return result;
}

/*----------- auxiliary data for caching terms -----------*/
static seq_t *cache      = NULL;   /* stored terms */
static size_t cache_size = 0;      /* current number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t last_checked = 0;     /* last integer examined */

/* Ensure the cache array has room for 'needed' entries */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    /* realloc failure is unlikely in the test harness */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether k satisfies the defining property */
static bool satisfies(idx_t k) {
    idx_t sigma_k      = sigma_idx(k);          /* σ(k) */
    idx_t omega_k      = (idx_t)bigomega(k);    /* Ω(k) */
    seq_t sigma_omega_k   = (seq_t)sigma_idx(omega_k); /* σ(Ω(k)) */
    seq_t omega_sigma_k   = bigomega(sigma_k);          /* Ω(σ(k)) */
    return sigma_omega_k == omega_sigma_k;
}

/* Public function: nth term of the sequence (offset 1) */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* sequence starts at offset 1 */

    /* generate terms until we have n of them */
    while (cache_size < (size_t)n) {
        do {
            ++last_checked;
        } while (!satisfies(last_checked));

        /* store the found value */
        ensure_cache_capacity(cache_size + 1);
        cache[cache_size++] = (seq_t)last_checked;
    }
    return cache[n - 1];
}
