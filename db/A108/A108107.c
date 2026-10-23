/*
 * A108107  Numbers n such that the string 1123n is the decimal expansion of a
 *          prime number.
 * https://oeis.org/A108107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the decimal string "1123"||k
 *            is a prime number
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- Miller–Rabin primality test for 64‑bit numbers ----- */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, cur = d;
        while (cur) {
            if (cur & 1) x = (x * p) % n;
            p = (p * p) % n;
            cur >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- Number of decimal digits of a 64‑bit integer ----- */
static inline int num_digits_uint64(uint64_t x)
{
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

/* ----- Precomputed powers of 10 up to 10^15 ----- */
static const uint64_t pow10[16] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL
};

/* ----- Storage for generated terms ----- */
static seq_t *terms      = NULL;
static size_t terms_sz   = 0;
static size_t terms_cap  = 0;

/* ----- Ensure capacity for at least min_cap terms ----- */
static void ensure_terms_capacity(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof *terms);
    if (!new_arr) abort();
    terms = new_arr;
    terms_cap = new_cap;
}

/* ----- Generate terms until we have at least n terms ----- */
static void generate_until(size_t n)
{
    if (terms_sz >= n) return;
    ensure_terms_capacity(n);

    static uint64_t last_candidate = 0;      /* highest candidate checked */

    while (terms_sz < n) {
        for (uint64_t cand = last_candidate + 1; ; ++cand) {
            int d = num_digits_uint64(cand);
            if (d >= sizeof pow10 / sizeof *pow10) { /* would overflow 64‑bit */
                last_candidate = cand;
                continue;
            }
            uint64_t base = pow10[d];
            __uint128_t full = (__uint128_t)1123ULL * base + cand;
            if (full > ( __uint128_t)UINT64_MAX) { /* overflow in 64‑bit */
                last_candidate = cand;
                continue;
            }
            uint64_t val = (uint64_t)full;
            if (is_prime_uint64(val)) {
                terms[terms_sz++] = (seq_t)cand;
                last_candidate = cand;
                break;
            }
        }
    }
}

/* ----- The required public function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for zero index */
    generate_until((size_t)n);
    return terms[(size_t)(n - 1)];
}
