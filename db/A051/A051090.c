/*
 * A051090  Primes p such that x^42 = -2 has a solution mod p.
 * https://oeis.org/A051090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that the congruence x^42 ≡ -2 (mod p) is solvable
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 972 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"
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

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 2;

    while (cache_len < n) {
        idx_t p = next_candidate;
        /* Skip even candidates > 2 */
        if (p > 2 && (p & 1) == 0) {
            next_candidate++;
            continue;
        }

        /* Check primality */
        if (!is_prime_big((seq_t)p)) {
            next_candidate++;
            continue;
        }

        bool good = false;
        if (p == 2) {
            /* 0 ≡ -2 (mod 2) is solvable via x = 0 */
            good = true;
        } else {
            /* Check if -2 is a 42nd power residue modulo p */
            idx_t g = gcd_idx(42, p - 1);
            idx_t exp = (p - 1) / g;   /* exponent for the test */
            seq_t target = (seq_t)p - 2; /* -2 modulo p (works for p > 2) */
            seq_t val = powmod(target, (uint64_t)exp, (seq_t)p);
            if (val == 1)
                good = true;
        }

        if (good) {
            /* Store the prime in the cache */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = (seq_t)p;
        }

        next_candidate++;
    }
    return cache[n - 1];
}
