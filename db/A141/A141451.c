/*
 * A141451  a(1) = 2. a(n) = the smallest integer > a(n-1) that is divisible
 *          by every exponent in the prime factorization of a(n-1).
 * https://oeis.org/A141451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2.
   For n>1, a(n) = ((a(n-1)/lcm(E) + 1) * lcm(E)),
   where E is the set of exponents in the prime factorization of a(n-1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/*---------------------------------- helpers ----------------------------------*/

static inline uint64_t lcm_uint64(uint64_t a, uint64_t b) {
    return (a / gcd_uint64(a, b)) * b;
}

/* factorization: returns the exponents in +exps[0..cnt-1] (cnt <= 64) */
static void factor_exponents(uint64_t n, uint64_t exps[64], size_t *cnt) {
    *cnt = 0;
    if (n == 0) return;
    uint64_t e = 0;
    if (n % 2 == 0) {
        e = 0;
        while (n % 2 == 0) { n /= 2; e++; }
        exps[(*cnt)++] = e;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) { n /= p; e++; }
            exps[(*cnt)++] = e;
        }
    }
    if (n > 1) exps[(*cnt)++] = 1;   /* remaining prime factor has exponent 1 */
}

/*--------------------------------- cache ------------------------------------*/
static seq_t *cache = NULL;          /* 1‑indexed array of a(n) values */
static size_t cache_cap = 0;         /* capacity of cache array */
static size_t cache_max = 0;         /* largest n for which a(n) is computed */

/* ensure the cache can hold indices up to n */
static void ensure_cache(size_t n) {
    if (n < cache_max) return;
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* in practice this will not happen for n<=10000 */
        cache = tmp;
        cache_cap = new_cap;
    }
    if (cache_max == 0) {          /* initialize a(1)=2 */
        cache[1] = (seq_t)2;
        cache_max = 1;
    }
    /* compute values up to n */
    for (size_t i = cache_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        /* cast prev to uint64_t for factoring (value fits in 64 bits here) */
        uint64_t v = (uint64_t)prev;
        uint64_t exps[64];
        size_t cnt = 0;
        factor_exponents(v, exps, &cnt);
        /* lcm of all exponents */
        uint64_t lcmExp = 1;
        for (size_t k = 0; k < cnt; ++k) lcmExp = lcm_uint64(lcmExp, exps[k]);
        seq_t lcmSeq = (seq_t)lcmExp;
        seq_t q   = prev / lcmSeq;        /* integer division in seq_t */
        cache[i]  = (q + 1) * lcmSeq;     /* next multiple > prev */
    }
    cache_max = n;
}

/*----------------------------------- public -----------------------------------*/
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety: offset is 1, but just in case */
    ensure_cache((size_t)n);
    return cache[(size_t)n];
}
