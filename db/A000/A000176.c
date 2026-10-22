/*
 * A000176  Generalized tangent numbers d_(n,2).
 * https://oeis.org/A000176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n > 1 put D = n if n == 1 (mod 4), else D = 4n,
 *            and e = sqrt(D/n). Write D = F*g^2, where F is the fundamental
 *            discriminant and chi is the Kronecker character (F/.). Then a(n)
 *            = -32 * (g/e)^7 * B_{4,chi} * Prod_{p | 2n, p not | F} (1 -
 *            chi(p)/p^4), where B_{4,chi} = (1/F) * Sum_{r=1...
 * Range:     u8 n <= 4, u16 n <= 23, u32 n <= 567, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a176_jacobi, a176_kron
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a176_jacobi.h"
#include "../utils/a176_kron.h"

typedef __int128 a176_i128;

static a176_i128 a176_abs(a176_i128 x) { return x < 0 ? -x : x; }

static a176_i128 a176_gcd(a176_i128 x, a176_i128 y)
{
    x = a176_abs(x); y = a176_abs(y);
    while (y != 0) { a176_i128 t = x % y; x = y; y = t; }
    return x;
}

static void a176_mul(a176_i128 *num, a176_i128 *den, a176_i128 x, a176_i128 y)
{
    a176_i128 g1 = a176_gcd(x, *den), g2;
    if (g1 > 1) { x /= g1; *den /= g1; }
    g2 = a176_gcd(*num, y);
    if (g2 > 1) { *num /= g2; y /= g2; }
    *num *= x;
    *den *= y;
}

seq_t a(idx_t n)
{
    uint64_t A = (uint64_t)n, D, e, tmp, s = 1, t = 1, F, g, p;
    uint64_t primes[64];
    int np = 0, i, has2 = 0;
    a176_i128 num, den, g0;
    seq_t res;
    uint64_t lo, hi;

    if (A == 1) return (seq_t)2;
    if (A % 4 == 1) { D = A; e = 1; } else { D = 4 * A; e = 2; }

    tmp = D;
    for (p = 2; p * p <= tmp; p++) {
        if (tmp % p == 0) {
            int cnt = 0;
            while (tmp % p == 0) { tmp /= p; cnt++; }
            if (cnt & 1) s *= p;
            for (i = 0; i < cnt / 2; i++) t *= p;
            primes[np++] = p;
            if (p == 2) has2 = 1;
        }
    }
    if (tmp > 1) {
        s *= tmp;
        primes[np++] = tmp;
        if (tmp == 2) has2 = 1;
    }
    if (!has2) primes[np++] = 2;

    if (s % 4 == 1) { F = s; g = t; } else { F = 4 * s; g = t / 2; }

    if (F == 1) {
        num = -1; den = 30;
    } else {
        a176_i128 S = 0;
        uint64_t r;
        for (r = 1; r < F; r++) {
            int c = a176_kron(F, r);
            if (c != 0) {
                a176_i128 q = (a176_i128)(r * (F - r));
                q = q * q;
                if (c > 0) S += q; else S -= q;
            }
        }
        num = S; den = (a176_i128)F;
        g0 = a176_gcd(num, den);
        if (g0 > 1) { num /= g0; den /= g0; }
    }

    a176_mul(&num, &den, -32, 1);
    for (i = 0; i < 7; i++) a176_mul(&num, &den, (a176_i128)g, (a176_i128)e);

    for (i = 0; i < np; i++) {
        uint64_t q = primes[i];
        a176_i128 p4;
        int c;
        if (F % q == 0) continue;
        c = a176_kron(F, q);
        p4 = (a176_i128)q * q * q * q;
        a176_mul(&num, &den, p4 - c, p4);
    }

    num /= den;
    if (num < 0) num = -num;

    lo = (uint64_t)num;
    hi = (uint64_t)((unsigned __int128)num >> 64);
    if (hi == 0) return (seq_t)lo;
    res = (seq_t)hi;
    res = res << 32;
    res = res << 32;
    res += (seq_t)lo;
    return res;
}
