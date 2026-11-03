/*
 * A104589  a(1)=1. a(n) = a(n-1) + (sum of terms, from among terms a(1)
 *          through a(n-1), which are prime or 1).
 * https://oeis.org/A104589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 66, u64 n <= 265, u128 n <= 546,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 1970 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A104589: a(1) = 1; a(n) = a(n-1) + the sum of those of a(1..n-1) that are
 * prime or 1. Computed from the definition in seq_t. Primality: the exact
 * 64-bit test when the value fits, else (only for big) Miller-Rabin in seq_t
 * to the first 20 prime bases, a probable-prime test (no counterexample is
 * known at these sizes). (The original tested values in uint64_t, which
 * truncates for big once the terms pass 2^64.) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static bool fits_u64(seq_t v)
{
    seq_t m = (seq_t)UINT64_MAX;
    return !(m < v);
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

static bool probable_prime(seq_t v)
{
    static const unsigned P[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    for (int i = 0; i < 20; ++i)
        if (v % (seq_t)P[i] == (seq_t)0) return v == (seq_t)P[i];
    seq_t d = v - (seq_t)1;
    unsigned s = 0;
    while (d % (seq_t)2 == (seq_t)0) { d = d / (seq_t)2; ++s; }
    for (int i = 0; i < 20; ++i) {
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

static bool is_prime_any(seq_t v)
{
    if (fits_u64(v)) return is_prime_u64((uint64_t)v);
    return probable_prime(v);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t cur = (seq_t)1, S = (seq_t)1;           /* a(1) = 1 counts (it is 1) */
    for (idx_t i = 2; i <= n; ++i) {
        cur = cur + S;
        if (is_prime_any(cur)) S = S + cur;
    }
    return cur;
}
