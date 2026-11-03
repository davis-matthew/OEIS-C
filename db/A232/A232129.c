/*
 * A232129  Largest prime that can be obtained from n by successively
 *          appending digits to the right with the constraint that each of the
 *          numbers obtained that way must be prime; a(n)=0 if there is no
 *          such prime at all.
 * https://oeis.org/A232129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest prime reachable by successively appending digits
 *            to the right such that all intermediate numbers stay prime;
 *            a(n)=0 if no such prime exists
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 9214 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

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

/* Deterministic Miller–Rabin primality test for 64‑bit unsigned integers */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
#if INFINITY
#else
    return (unsigned __int128)a * b % mod;
#endif
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    uint64_t base = a % mod;
    while (e) {
        if (e & 1U) res = mod_mul64(res, base, mod);
        base = mod_mul64(base, base, mod);
        e >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    size_t i;
    for (i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1U) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t test_bases[] = {2,3,5,7,11,13,17};
    size_t j;
    for (j = 0; j < sizeof(test_bases)/sizeof(test_bases[0]); ++j) {
        uint64_t a = test_bases[j];
        if (a % n == 0) return true;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        unsigned r;
        for (r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static inline bool seq_is_prime(seq_t n)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        return is_prime_uint64((uint64_t)n);
    } else {
        /* 128‑bit or arbitrary‑precision types use the supplied routine. */
        return is_prime_big(n);
    }
}

/* Recursive search for the largest prime reachable from cur. */
static seq_t best_prime_from(seq_t cur)
{
    seq_t best = 0;
    bool found_child = false;

    for (int d = 0; d < 10; ++d) {
        seq_t nxt = cur * 10 + (seq_t)d;
        if (!seq_is_prime(nxt)) continue;
        found_child = true;
        seq_t cand = best_prime_from(nxt);
        if (cand > best) best = cand;
    }

    if (!found_child) {
        return seq_is_prime(cur) ? cur : (seq_t)0;
    }
    return best;
}

seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;
    return best_prime_from(start);
}
