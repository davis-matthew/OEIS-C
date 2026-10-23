/*
 * A177089  Numbers k >= 0 such that k^3-k-1 is not prime.
 * https://oeis.org/A177089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* A177089: numbers k ≥ 0 such that k³ – k – 1 is not prime. */
/* The implementation keeps a static cache of the sequence values and
 * extends it on demand. The primality test uses a deterministic
 * Miller‑Rabin routine for uint64_t, which is sufficient for all k
 * that can appear in the first 10 000 terms. */

/* ----------------------------------------------------------- */
/* Deterministic Miller–Rabin for 64‑bit integers                */
/* ----------------------------------------------------------- */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul_uint64(r, a, m);
        a = mod_mul_uint64(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37,41};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* bases for deterministic test for n < 2^64 */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ----------------------------------------------------------- */
/* The public sequence function                                 */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t count = 0;    /* number of cached terms */
    static idx_t k = 0;        /* next k to test */

    /* grow cache until we have at least n terms */
    while (count < n) {
        unsigned __int128 kk = (unsigned __int128)k;
        unsigned __int128 expr = kk * kk * kk - kk - 1;
        uint64_t val = (uint64_t)expr;        /* fits for k up to ~10⁴ */

        if (!is_prime_u64(val)) {
            /* ensure space */
            if (count >= cap) {
                size_t new_cap = cap ? cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;        /* allocation failed */
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = (seq_t)k;
        }
        ++k;
    }

    return cache[n - 1];
}
