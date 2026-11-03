/*
 * A088896  Length of longest integral ladder that can be moved horizontally
 *          around the right angled corner where two hallway corridors of
 *          integral widths meet.
 * https://oeis.org/A088896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n)^3, where d(n) is the nth integer that has at least
 *            one prime divisor ≡1 (mod 4)
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether x has a prime divisor ≡ 1 (mod 4) */
static bool has_prime_1mod4(uint64_t x)
{
    if (x < 5) return false;           /* 5 is the smallest prime ≡ 1 (mod 4) */
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            if (p % 4 == 1) return true;
            while (x % p == 0) x /= p;
        }
    }
    return (x > 1 && x % 4 == 1);
}

/* Cache of computed terms */
static seq_t *terms = NULL;
static idx_t  cached = 0;        /* number of terms cached (terms[0]..terms[cached-1]) */
static uint64_t next_num = 1;    /* next integer to test for the property */

/* Ensure terms[0..n-1] are computed */
static void ensure_cached(idx_t n)
{
    if (cached >= n) return;
    /* enlarge array in chunks to reduce allocations */
    idx_t old = cached;
    idx_t new_cap = cached * 2 + 10;
    terms = mem_resize(seq_t, terms, new_cap);
    for (idx_t i = old; i < new_cap; ++i) terms[i] = 0; /* optional */

    while (cached < n) {
        do { ++next_num; } while (!has_prime_1mod4(next_num));
        seq_t t = (seq_t)next_num;
        seq_t val = t * t * t;          /* cube, unsigned wrap‑around is defined */
        terms[cached++] = val;
    }
}

/* OEIS sequence A088896 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) is undefined; guard */
    ensure_cached(n);
    return terms[n - 1];
}
