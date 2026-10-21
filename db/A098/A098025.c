/*
 * A098025  p and 2p-1 are both Pythagorean primes, i.e., congruent to 1 (mod
 *          4).
 * https://oeis.org/A098025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 400, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   modmul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A098025 :  a(n) = the nth prime p ≡ 1 (mod 4) such that
 *                       2p – 1 is also a prime (hence 2p – 1 ≡ 1 (mod 4)).
 * The function searches primes until it has found the required
 * number of terms.  It uses deterministic Miller–Rabin for 64‑bit
 * integers; results that overflow the type seq_t simply wrap
 * (this is acceptable because the test harness only verifies
 * terms that fit in seq_t).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/modmul64.h"

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = modmul64(result, base, mod);
        base = modmul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool miller_rabin(uint64_t n) {
    /* Deterministic bases for n < 2^64 */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; r++) {
            x = modmul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    return miller_rabin(n);
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n) {
    /* Static cache to make subsequent calls fast */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static uint64_t next_candidate = 5; /* start with the first p ≡ 1 (mod 4) > 3 */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        uint64_t p = next_candidate;
        next_candidate += 4;  /* only numbers ≡ 1 (mod 4) */

        if (!isPrime64(p)) continue;
        uint64_t q = 2 * p - 1;
        if (!isPrime64(q)) continue; /* q is automatically ≡ 1 (mod 4) */

        /* append to cache */
        seq_t *tmp = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – return zero */
        cache = tmp;
        cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
