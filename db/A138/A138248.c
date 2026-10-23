/*
 * A138248  Prime numbers k such that 18*k - 1 and 18*k + 1 are twin primes.
 * https://oeis.org/A138248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes k such that 18*k‑1 and 18*k + 1 are twin primes.
   The sequence is built lazily; subsequent calls reuse cached values. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller–Rabin ---------- */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    __uint128_t r = (__uint128_t)a * b % mod;
    return (uint64_t)r;
}

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1)
            res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    uint64_t bases[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* cached sequence values           */
    static size_t cache_len = 0;     /* number of cached values          */
    static size_t cache_cap = 0;     /* allocated capacity               */
    static uint64_t candidate = 11;  /* next odd candidate (prime range) */

    /* Ensure cache holds at least n elements (1‑based index) */
    while (cache_len < (size_t)n) {
        for (;;) {
            /* Test candidate */
            if (is_prime_uint64(candidate)) {
                uint64_t m1 = 18ULL * candidate - 1;
                uint64_t m2 = 18ULL * candidate + 1;
                if (is_prime_uint64(m1) && is_prime_uint64(m2)) {
                    /* Store the value */
                    if (cache_len + 1 > cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp) abort();   /* allocation failed */
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)candidate;
                    candidate += 2;  /* advance to next odd number */
                    break;           /* ready for next required element */
                }
            }
            candidate += 2;       /* skip even numbers */
        }
    }

    return cache[(size_t)(n - 1)];   /* 1‑based indexing */
}
