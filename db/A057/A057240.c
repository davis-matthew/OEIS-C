/*
 * A057240  Numbers k such that k | 11^k + 10^k + 9^k + 8^k.
 * https://oeis.org/A057240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   modmul_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057240: a(n) is the n‑th positive integer k such that
 * k divides 11^k + 10^k + 9^k + 8^k.
 * The routine performs a linear search, testing each k with
 * modular exponentiation using 64‑bit arithmetic and an
 * __uint128_t intermediate for safe multiplication. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/modmul_uint64.h"

/* 64‑bit modular exponentiation */
static inline idx_t modpow_uint64(idx_t base, uint64_t exp, idx_t mod) {
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = modmul_uint64(result, b, mod);
        b = modmul_uint64(b, b, mod);
        exp >>= 1U;
    }
    return result;
}

/* Test whether a given k satisfies k | 11^k + 10^k + 9^k + 8^k */
static inline bool good_k(idx_t k) {
    idx_t mod = k;
    idx_t sum = modpow_uint64(11, k, mod);
    sum += modpow_uint64(10, k, mod);
    if (sum >= mod) sum -= mod;
    sum += modpow_uint64(9, k, mod);
    if (sum >= mod) sum -= mod;
    sum += modpow_uint64(8, k, mod);
    if (sum >= mod) sum -= mod;
    return sum % mod == 0;
}

/* Sequence generator with memoisation */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset starts at 1 */

    static seq_t *cache = NULL;
    static size_t  len   = 0;
    static size_t  cap   = 0;
    static idx_t   next_k = 1;        /* next integer to test */

    if (cap == 0) {                  /* first allocation */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        /* find the next k satisfying the condition */
        while (!good_k(next_k))
            ++next_k;

        seq_t val = (seq_t)next_k;   /* cast to seq_t */

        /* If val cannot represent the actual number, terminate */
        if ((idx_t)val != next_k)
            return 0;                /* requested term is too large for this build */

        /* grow cache if necessary */
        if (len == cap) {
            cap <<= 1;                /* double capacity */
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len++] = val;
        ++next_k;                     /* advance for next search */
    }

    return cache[n - 1];              /* a(1) is at index 0 */
}
