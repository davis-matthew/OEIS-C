/*
 * A107160  Primes of the form x^2 + 52y^2.
 * https://oeis.org/A107160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as x^2 + 52*y^2 (y >
 *            0)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* ---------- helper functions ---------- */

static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1U) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1U;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime(uint64_t n) {
    if (n < 2U) return false;
    static const uint64_t small[12] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0U; i < sizeof small / sizeof *small; ++i) {
        uint64_t p = small[i];
        if (n % p == 0U) return n == p;
    }

    uint64_t d = n - 1U;
    unsigned int s = 0U;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    static const uint64_t bases[7] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0U; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;
        bool composite = true;
        for (unsigned int r = 1U; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1ULL) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Integer square root, 0 <= n <= UINT64_MAX */
static uint64_t isqrt64(uint64_t n) {
    uint64_t r = 0U;
    uint64_t bit = 1ULL << 62U; /* highest power of 4 <= 2^64 */
    while (bit) {
        uint64_t t = r | bit;
        if ((unsigned __int128)t * t <= n) r = t;
        bit >>= 1U;
    }
    return r;
}

static bool is_square_uint64(uint64_t n) {
    uint64_t r = isqrt64(n);
    return (unsigned __int128)r * r == n;
}

/* Check whether prime p can be written as x^2 + 52*y^2 (y > 0) */
static bool representable(uint64_t p) {
    if (p < 53ULL) return false;   /* smallest possible is 53 */
    uint64_t max_y = isqrt64(p / 52ULL);
    for (uint64_t y = 1ULL; y <= max_y; ++y) {
        unsigned __int128 val = (unsigned __int128)52ULL * y * y;
        uint64_t diff = (uint64_t)((unsigned __int128)p - val);
        if (is_square_uint64(diff)) return true;
    }
    return false;
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n) {
    static uint64_t *vals = NULL;   /* buffer for found primes */
    static size_t cap  = 0;         /* current capacity */
    static size_t sz   = 0;         /* number of stored primes */
    static uint64_t cur = 2ULL;     /* current integer to test */

    /* grow buffer as needed */
    if (cap == 0) {
        cap = 8;
        vals = (uint64_t *)malloc(cap * sizeof *vals);
        if (!vals) return (seq_t)0; /* allocation failure */
    }

    while (sz < (size_t)n) {
        ++cur;
        if (!is_prime(cur)) continue;
        if (representable(cur)) {
            if (sz >= cap) {
                cap = cap * 2U;
                uint64_t *tmp = (uint64_t *)realloc(vals, cap * sizeof *vals);
                if (!tmp) { free(vals); return (seq_t)0; }
                vals = tmp;
            }
            vals[sz++] = cur;
        }
    }
    return (seq_t)vals[n - 1U];
}
