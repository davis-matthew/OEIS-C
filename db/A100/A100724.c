/*
 * A100724  Prime numbers whose binary representations are split into at most
 *          3 runs.
 * https://oeis.org/A100724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 134, u32 n <= 613, u64 n <= 2533,
 *            u128 n <= 3222, big any n
 * Validated: standard, pass, 71 terms through n = 9830 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A number with at most 3 binary runs is 1^a (2^a - 1), 1^a 0^b (even, so
 * only 2 = "10" is prime) or 1^a 0^b 1^c. For L bits, in increasing order,
 * these are 1^a 0^b 1^c for a = 1..L-2 and, for each a, c = 1..L-a-1, then
 * 2^L - 1: (2^a - 1) 2^(L-a) grows with a by more than any 2^c - 1 below
 * it. So the candidates come in order with no sorting, and are tested for
 * primality: exactly when they fit in 64 bits, and above that (only big
 * gets there; the terms reach 4e37) by Miller-Rabin in seq_t to the first 20
 * prime bases, a probable-prime test (no counterexample is known at these
 * sizes). Widths narrower than the values stop with 0. (The original tried
 * every odd number by trial division, and in 64 bits.) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

static unsigned width(void)                 /* bits seq_t holds; 1000 for big */
{
    if (sizeof(seq_t) < 16) return 8 * sizeof(seq_t);
    seq_t x = (seq_t)1 << 64;
    x = x << 64;
    return x == (seq_t)0 ? 128 : 1000;
}

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
    static const unsigned P[20] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
    for (int i = 0; i < 20; ++i)
        if (v % (seq_t)P[i] == (seq_t)0) return false;
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

static seq_t *terms = NULL;
static size_t cnt = 0, cap = 0;

static void add(seq_t v)
{
    if (cnt == cap) {
        cap = cap ? 2 * cap : 256;
        seq_t *t = mem_resize(seq_t, terms, cap);
        if (!t) abort();
        terms = t;
    }
    terms[cnt++] = v;
}

seq_t a(idx_t n)
{
    static unsigned L = 1;                  /* lengths < L are done */
    if (n == 0) return (seq_t)0;
    unsigned w = width();
    while (cnt < (size_t)n) {
        ++L;
        if (L > w) return (seq_t)0;         /* beyond this width */
        if (L == 2) add((seq_t)2);          /* "10" */
        seq_t one = (seq_t)1;
        for (unsigned A = 1; A + 2 <= L; ++A) {
            seq_t high = ((one << A) - one) << (L - A);
            for (unsigned c = 1; c + A + 1 <= L; ++c) {
                seq_t v = high + ((one << c) - one);
                if (is_prime_any(v)) add(v);
            }
        }
        seq_t mers = L == w ? (seq_t)0 - one : (one << L) - one;   /* (no shift by the full width) */
        if (is_prime_any(mers)) add(mers);
    }
    return terms[n - 1];
}
