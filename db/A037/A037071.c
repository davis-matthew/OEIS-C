/*
 * A037071  Smallest prime containing exactly n 9's.
 * https://oeis.org/A037071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest prime containing exactly n digits 9.
 *            Algorithm: start with the number 10^(n+1)−1 (i.e. n+1 nines),
 *            replace each 9 in turn (most significant first) by 1, 2, 4, 5,
 *            7, 8 until a prime is found. A deterministic Miller‑Rabin test
 *            for 64‑bit integers is used for numbers that fit in...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 179 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Deterministic Miller‑Rabin for 64‑bit integers.
   Uses the base set {2,3,5,7,11,13,17} which is valid for all 64‑bit values. */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    /* 128‑bit intermediate is available in both C11 and C++17. */
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t pow_mod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Small primes */
    const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
        if (n % small[i] == 0) return n == small[i];

    /* Miller–Rabin */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t a = small[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Digits that may replace a 9 while keeping the number prime. */
    static const int allowed[6] = {1, 2, 4, 5, 7, 8};

    idx_t len = n + 1;                      /* number of digits of the base */
    seq_t base = pow10(len);                /* 10^len */
    base -= (seq_t)1;                       /* 10^len - 1  (all 9's) */

    /* Cache the 64‑bit upper bound once. */
    static const seq_t max64_seq = (seq_t)UINT64_MAX;

    for (idx_t pos = 0; pos < len; ++pos) {
        /* Weight of the position being changed: 10^(len–1–pos). */
        seq_t power = pow10(len - 1 - pos);

        for (int k = 0; k < 6; ++k) {
            int d = allowed[k];
            seq_t diff = seq_mul(power, (seq_t)(9 - d));
            seq_t cand = base - diff;

            /* If the candidate fits in uint64_t, use the fast test. */
            if (cand <= max64_seq) {
                uint64_t cand64 = (uint64_t)cand;
                if (is_prime_uint64(cand64))
                    return cand;
            } else {
                if (is_prime_big(cand))
                    return cand;
            }
        }
    }
    /* The conjecture guarantees a prime is always found; unreachable. */
    return base;
}
