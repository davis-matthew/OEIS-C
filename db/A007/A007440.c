/*
 * A007440  Reversion of g.f. for Fibonacci numbers 1, 1, 2, 3, 5, ....
 * https://oeis.org/A007440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   (n+3)*a(n+2) = -(2*n+3)*a(n+1) - 5*n*a(n), a(1) = 1, a(2) = -1
 * Range:     i8 n <= 11, i16 n <= 18, i32 n <= 34, i64 n <= 63, i128 n <= 119,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 784 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

/* Arbitrary-precision build: the recurrence applied directly in seq_t. */
static seq_t a007440_big(idx_t n) {
    seq_t pp = 1, p = -1;
    for (idx_t k = 1; k + 2 <= n; k++) {
        seq_t c1 = (seq_t)k * 2 + 3;
        seq_t c2 = (seq_t)k * 5;
        seq_t d = (seq_t)k + 3;
        seq_t c = (-(c1 * p) - c2 * pp) / d;
        pp = p;
        p = c;
    }
    return p;
}

/* Fixed-width builds: 128-bit arithmetic. Each step divides p and pp by d
   first, so the only exact division is on small remainders. The big
   products are summed modulo 2^128, which is exact whenever the result
   fits. */
static seq_t a007440_fixed(idx_t n) {
    __int128 pp = 1, p = -1;
    for (idx_t k = 1; k + 2 <= n; k++) {
        __int128 c1 = (__int128)k * 2 + 3;
        __int128 c2 = (__int128)k * 5;
        __int128 d = (__int128)k + 3;
        __int128 q1 = p / d, r1 = p % d;
        __int128 q2 = pp / d, r2 = pp % d;
        __int128 s = (c1 * r1 + c2 * r2) / d;
        unsigned __int128 t = (unsigned __int128)c1 * (unsigned __int128)q1
                            + (unsigned __int128)c2 * (unsigned __int128)q2
                            + (unsigned __int128)s;
        t = (unsigned __int128)0 - t;
        pp = p;
        p = (__int128)t;
    }
    return from_i128(p);
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return -1;
    if (seq_is_fixed_width()) return a007440_fixed(n);
    return a007440_big(n);
}
