/*
 * A002554  Numerators of coefficients for numerical differentiation.
 * https://oeis.org/A002554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator((-1)^(n-1) * (2n-1)!! * S(n) / (n! *
 *            2^(3n-3))), where S(n) = Sum_{j=1..n} 1/(2j-1)^2. This is the
 *            same as numerator(A001824(n-1)*(-1)^(n-1)/(2^(2n-3)*(2n)!)),
 *            because A001824(n-1) = ((2n-1)!!)^2 * S(n) and (2n)! = (2n-1)!!
 *            * 2^n * n!. S(n) = num/L with L = lcm(1^2,3^2,...,(2n-1)...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#define A2554_FIXED (!std::is_class<seq_t>::value)
#else
#define A2554_FIXED 1
#endif

typedef unsigned __int128 a2554_u128;

static uint64_t a2554_leg(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) { m /= p; s += m; }
    return s;
}

static uint32_t *a2554_spf(uint64_t N) {
    uint32_t *s = (uint32_t *)calloc((size_t)N + 1, sizeof(uint32_t));
    if (!s) return s;
    for (uint64_t i = 2; i <= N; i++)
        if (s[i] == 0)
            for (uint64_t j = i; j <= N; j += i)
                if (s[j] == 0) s[j] = (uint32_t)i;
    return s;
}

/* growth factor of L when (2k-1)^2 joins: p^2 if 2k-1 = p^e, else 1 */
static uint64_t a2554_grow(uint64_t m, const uint32_t *spf) {
    uint64_t p = spf[m], x = m;
    while (x % p == 0) x /= p;
    return x == 1 ? p * p : 1;
}

/* (exponent of p in denominator) - (exponent of p in (2n-1)!!) */
static long long a2554_excess(uint64_t n, uint64_t p) {
    if (p == 2) return (long long)(a2554_leg(n, 2) + 3 * n - 3);
    uint64_t vL = 0, pw = 1;
    while (pw <= (2 * n - 1) / p) { pw *= p; vL += 2; }
    long long den = (long long)(vL + a2554_leg(n, p));
    long long num = (long long)(a2554_leg(2 * n, p) - a2554_leg(n, p));
    return den - num;
}

static a2554_u128 a2554_fixed_mag(uint64_t n, const uint32_t *spf) {
    a2554_u128 L = 1, num = 1, mult = 1;
    for (uint64_t k = 2; k <= n; k++) {
        uint64_t m = 2 * k - 1;
        uint64_t f = a2554_grow(m, spf);
        L *= f;
        num = num * f + L / ((a2554_u128)m * m);
    }
    for (uint64_t p = 2; p <= 2 * n; p++) {
        if (spf[p] != p) continue;
        long long t = a2554_excess(n, p);
        if (t < 0) {
            for (long long i = 0; i < -t; i++) mult *= p;
        } else {
            while (t-- > 0 && num != 0 && num % p == 0) num /= p;
        }
    }
    return num * mult;
}

static seq_t a2554_big(uint64_t n, const uint32_t *spf) {
    seq_t L = 1, num = 1, mult = 1;
    for (uint64_t k = 2; k <= n; k++) {
        uint64_t m = 2 * k - 1;
        uint64_t f = a2554_grow(m, spf);
        if (f != 1) {
            L *= (seq_t)f;
            num *= (seq_t)f;
        }
        num += L / (seq_t)(m * m);
    }
    for (uint64_t p = 2; p <= 2 * n; p++) {
        if (spf[p] != p) continue;
        long long t = a2554_excess(n, p);
        if (t < 0) {
            for (long long i = 0; i < -t; i++) mult *= (seq_t)p;
        } else {
            seq_t sp = (seq_t)p;
            while (t-- > 0 && num % sp == 0) num /= sp;
        }
    }
    return num * mult;
}

static seq_t a2554_from_u128(a2554_u128 u) {
    seq_t r = 0;
    for (int sh = 96; sh >= 0; sh -= 32) {
        uint32_t chunk = (uint32_t)(u >> sh);
        r = r * 65536 * 65536 + (seq_t)(long long)chunk;
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t nn = (uint64_t)n;
    uint32_t *spf = a2554_spf(2 * nn);
    if (!spf) return 0;
    seq_t r;
    if (A2554_FIXED)
        r = a2554_from_u128(a2554_fixed_mag(nn, spf));
    else
        r = a2554_big(nn, spf);
    free(spf);
    if ((nn & 1) == 0) r = -r;
    return r;
}
