/*
 * A064415  a(1) = 0, a(n) = iter(n) if n is even, a(n) = iter(n)-1 if n is
 *          odd, where iter(n) = A003434(n) = smallest number of iterations of
 *          Euler totient function phi needed to reach 1.
 * https://oeis.org/A064415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for prime p: a(p)=1+a((p-1)/2); for composite n:
 *            a(n)=∑_{p^e‖n} e * a(p-1). The sequence is completely additive
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Cache structures */
static idx_t *cache_idx = NULL;
static seq_t *cache_val = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static bool cache_get(idx_t n, seq_t *out) {
    for (size_t i = 0; i < cache_len; ++i) {
        if (cache_idx[i] == n) {
            *out = cache_val[i];
            return true;
        }
    }
    return false;
}

static void cache_put(idx_t n, seq_t v) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        idx_t *new_idx = (idx_t *)realloc(cache_idx, new_cap * sizeof(idx_t));
        seq_t *new_val = (seq_t *)realloc(cache_val, new_cap * sizeof(seq_t));
        if (!new_idx || !new_val) exit(1);
        cache_idx = new_idx;
        cache_val = new_val;
        cache_cap = new_cap;
    }
    cache_idx[cache_len] = n;
    cache_val[cache_len] = v;
    ++cache_len;
}

/* Internal recursive computation with caching */
static seq_t compute_a(idx_t n) {
    if (n == 1) return 0;

    seq_t cached;
    if (cache_get(n, &cached)) return cached;

    /* Detect prime via factorization */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t fcnt;
    factor_u64((uint64_t)n, pr, exp, &fcnt);

    seq_t result;
    if (fcnt == 1 && pr[0] == n && exp[0] == 1) {          /* prime */
        if (n == 2) {
            result = 1;
        } else {
            idx_t half = (n - 1) / 2;
            result = 1 + compute_a(half);
        }
    } else {                                 /* composite */
        result = 0;
        for (size_t i = 0; i < fcnt; ++i) {
            seq_t term = compute_a(pr[i]);           /* a(p-1) for prime factor p */
            result += (exp[i] * term);
        }
    }

    cache_put(n, result);
    return result;
}

/* Public entry point */
seq_t a(idx_t n) {
    return compute_a(n);
}
