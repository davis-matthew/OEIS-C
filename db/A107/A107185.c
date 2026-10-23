/*
 * A107185  Primes of the form 4x^2 + 19y^2.
 * https://oeis.org/A107185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A107185 – Primes of the form 4·x² + 19·y²
 *  Computed by brute‑force primality testing (deterministic
 *  Miller–Rabin for 64‑bit integers) and a direct check that a
 *  prime p can be expressed as 4x² + 19y², i.e. that there is an
 *  odd y with (p – 19·y²)/4 a perfect square.
 *
 *  The function keeps a static cache of found values so that
 *  subsequent calls are O(1) for already discovered terms; new
 *  terms are generated on demand by scanning odd integers,
 *  testing for primality and for the representation condition.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ------------------  helpers ----------------------------------- */

static uint64_t isqrt64(uint64_t n) {
    uint64_t r = (uint64_t)sqrt((long double)n);   /* initial guess  */
    while ((unsigned __int128)(r + 1) * (r + 1) <= n) ++r;
    while ((unsigned __int128)r * r > n) --r;
    return r;
}

static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod) {
    unsigned __int128 res  = 1;
    unsigned __int128 base = a % mod;
    while (e) {
        if (e & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint64_t) res;
}

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                    19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static bool has_representation(uint64_t p) {
    /* p must be odd and ≡3 (mod 4) – skip check here for speed */
    for (uint64_t y = 1; ; y += 2) {         /* y odd only */
        unsigned __int128 val = (unsigned __int128)19 * y * y;
        if (val > p) break;
        uint64_t rem = p - (uint64_t)val;
        if (rem & 3ULL) continue;              /* rem % 4 != 0 */
        uint64_t t   = rem >> 2;                /* t = (p-19y²)/4   */
        uint64_t s   = isqrt64(t);
        if ((unsigned __int128)s * s == t) return true;
    }
    return false;
}

/* -------------------  main sequence -------------------------------- */

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t candidate = 3;   /* next odd to test */

    if (n == 0) return 0;   /* safety – offset 1 is defined for n >=1 */

    /* grow cache until it holds at least n terms */
    while (terms_len < (size_t)n) {
        if (isPrime64(candidate)) {
            if (has_representation(candidate)) {
                if (terms_len + 1 > terms_cap) {
                    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;              /* allocation fail */
                    terms = tmp; terms_cap = new_cap;
                }
                terms[terms_len++] = (seq_t)candidate;
            }
        }
        candidate += 2;   /* skip even numbers */
    }

    return terms[n - 1];
}
