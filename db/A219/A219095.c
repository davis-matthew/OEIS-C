/*
 * A219095  Numbers k such that if b/c > 1 is the least reduced fraction using
 *          divisors b and c of k, then c > 1.
 * https://oeis.org/A219095
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 5066, u32 n <= 5066, u64 n <= 5066,
 *            u128 n <= 5066, big any n
 * Helpers:   ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) is the n‑th integer k≥2 for which, among all reduced fractions b/c>1
 * with b and c divisors of k, the smallest such fraction has denominator >1.
 * The function enumerates integers k, builds all their divisors, finds the minimal
 * reduced ratio among consecutive divisors, and checks the denominator condition.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ugcd_uint64.h"

/* Returns true iff k satisfies the sequence condition. */
static bool good_uint64(uint64_t k) {
    if (k < 2) return false;

    /* Prime factorisation of k */
    uint64_t tmp = k;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pc = 0;

    if (tmp % 2 == 0) {
        uint64_t e = 0;
        while (tmp % 2 == 0) { tmp /= 2; ++e; }
        primes[pc] = 2;  exps[pc] = e;  ++pc;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) { tmp /= p; ++e; }
            primes[pc] = p;  exps[pc] = e;  ++pc;
        }
    }
    if (tmp > 1) { primes[pc] = tmp; exps[pc] = 1; ++pc; }

    /* Generate all divisors */
    uint64_t divs[256];
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < pc; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        size_t old = dcnt;
        for (size_t j = 0; j < old; ++j) {
            uint64_t base = divs[j];
            uint64_t pow  = 1;
            for (uint64_t k = 1; k <= e; ++k) {
                pow *= p;
                divs[dcnt++] = base * pow;
            }
        }
    }

    /* Sort divisors (simple insertion sort; array is very small). */
    for (size_t i = 1; i < dcnt; ++i) {
        uint64_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j-1] > key) {
            divs[j] = divs[j-1];
            --j;
        }
        divs[j] = key;
    }

    /* Find minimal reduced ratio among consecutive divisors. */
    uint64_t best_num = 0, best_den = 0;
    bool first = true;
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        uint64_t a = divs[i];
        uint64_t b = divs[i+1];
        if (b <= a) continue;      /* safety check */
        uint64_t g   = ugcd_uint64(a, b);
        uint64_t num = b / g;       /* reduced numerator   */
        uint64_t den = a / g;       /* reduced denominator */
        if (first || num * best_den < best_num * den) {
            best_num = num;
            best_den = den;
            first = false;
        }
    }

    /* Denominator must be > 1. */
    return best_den > 1;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* behaviour below the offset is irrelevant */

    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static uint64_t next_k  = 2;

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure (unlikely) */
        cache[0] = 0;
        cache[1] = 0;
        cache_len = 2;
    }

    while (cache_len <= n) {
        if (good_uint64(next_k)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
            }
            cache[cache_len] = (seq_t)next_k;
            ++cache_len;
        }
        ++next_k;
    }

    return cache[n];
}
