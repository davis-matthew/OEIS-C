/*
 * A104301  Primes which are the reverse concatenation of two consecutive
 *          square numbers.
 * https://oeis.org/A104301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime that equals reverse( reverse(m²) |
 *            reverse((m+1)²) ), for m = 1,2,3,…
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 18, u64 n <= 1915,
 *            u128 n <= 1400, big any n
 * Validated: standard, pass, 41 terms through n = 7774 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Reversing the concatenation of the reversed squares m^2, (m+1)^2 gives
   (m+1)^2 written before m^2: c = (m+1)^2 10^d + m^2, d the number of digits
   of m^2. The primes among these, in order of m (c grows with m), are the
   terms. Primality: the exact 64-bit test when c fits, else (only big gets
   there; the terms pass 2^64) Miller-Rabin in seq_t to the first 13 prime
   bases, which is deterministic below 3.3e24. (The original did all of this
   through decimal strings and the generic seq_t test, which timed out under
   big.) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static bool fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static seq_t powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = (seq_t)1;
    b = b % m;
    while (e > (seq_t)0) {
        if (e % (seq_t)2 == (seq_t)1) r = r * b % m;
        b = b * b % m;
        e = e / (seq_t)2;
    }
    return r;
}

static bool is_prime_any(seq_t v)
{
    if (fits_u64(v)) return is_prime_u64((uint64_t)v);
    static const unsigned P[13] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};
    for (int i = 0; i < 13; ++i)
        if (v % (seq_t)P[i] == (seq_t)0) return false;
    seq_t d = v - (seq_t)1;
    unsigned s = 0;
    while (d % (seq_t)2 == (seq_t)0) { d = d / (seq_t)2; ++s; }
    for (int i = 0; i < 13; ++i) {
        seq_t x = powmod((seq_t)P[i], d, v);
        if (x == (seq_t)1 || x == v - (seq_t)1) continue;
        bool ok = false;
        for (unsigned r = 1; r < s; ++r) {
            x = x * x % v;
            if (x == v - (seq_t)1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len = 0, cap = 0;
    static uint64_t m = 1;                    /* the next m to examine */
    if (n == 0) return (seq_t)0;
    while (len < (size_t)n) {
        uint64_t s1 = m * m, s2 = (m + 1) * (m + 1);
        seq_t pw = (seq_t)1;
        for (uint64_t t = s1; t; t /= 10) pw = pw * (seq_t)10;
        seq_t c = (seq_t)s2 * pw + (seq_t)s1;
        ++m;
        if (!is_prime_any(c)) continue;
        if (len == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *t = mem_resize(seq_t, cache, cap);
            if (!t) abort();
            cache = t;
        }
        cache[len++] = c;
    }
    return cache[n - 1];
}
