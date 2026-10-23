/*
 * A106025  Primes p such that 28*p +- 33, 38*p +- 33 and 58*p +- 33 are all
 *          primes.
 * https://oeis.org/A106025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 28*p±33, 38*p±33 and 58*p±33
 *            are all prime
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

/* 64‑bit Miller–Rabin primality test (deterministic for n < 2^64) */

static inline uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}
static inline bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t witness[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(witness)/sizeof(witness[0]); ++i) {
        uint64_t a = witness[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---- Sequence storage ---- */
static seq_t *terms = NULL;      /* array of found terms */
static idx_t term_count = 0;      /* number of terms stored so far */
static size_t term_capacity = 0;  /* allocated capacity */

/* next candidate prime that has not yet been tested */
static uint64_t candidate = 2;

/* ensure that at least n terms are available */
static void ensure_terms(idx_t n) {
    if (!terms) {
        term_capacity = 8;
        terms = (seq_t*)malloc(term_capacity * sizeof(seq_t));
    }
    while (term_count < n) {
        /* test current candidate */
        if (isPrime64(candidate)) {
            bool ok = true;
            static const int coeffs[] = {28, 38, 58};
            for (size_t i = 0; i < sizeof(coeffs)/sizeof(coeffs[0]); ++i) {
                uint64_t c = coeffs[i];
                uint64_t v1 = c * candidate - 33;
                uint64_t v2 = c * candidate + 33;
                if (!isPrime64(v1) || !isPrime64(v2)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                if (term_count == term_capacity) {
                    term_capacity *= 2;
                    terms = (seq_t*)realloc(terms, term_capacity * sizeof(seq_t));
                }
                terms[term_count++] = (seq_t)candidate;
            }
        }
        /* advance to next candidate */
        candidate = (candidate == 2) ? 3 : candidate + 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined for n < 1 */
    ensure_terms(n);
    return terms[n-1];
}
