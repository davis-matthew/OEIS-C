/*
 * A000061  Generalized tangent numbers d(n,1).
 * https://oeis.org/A000061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n,1) (Shanks). Write 2n's prime set P and n = b*m^2
 *            with b squarefree. b = 1: a(n) = (2/3) m^3 prod_{p|2n} (1 -
 *            1/p^2) b == 1 (mod 4): a(n) = (4 m^3 / b) * sum_{r=1..b-1} (r|b)
 *            r^2 * prod_{p|2n, p∤b} (p^2 - (p|b))/p^2 otherwise: a(n) = (m^3
 *            / (8b)) * sum_{r odd < 4b} (b|r) r^2 * prod_{p|...
 * Range:     u8 n <= 63, u16 n <= 2670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jac
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/jac.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    long long N = (long long)n, rem = N, b = 1, m = 1;
    long long primes[64];
    int np = 0;
    for (long long p = 2; p * p <= rem; p++) {
        if (rem % p) continue;
        int e = 0;
        while (rem % p == 0) { rem /= p; e++; }
        primes[np++] = p;
        if (e & 1) b *= p;
        for (int i = 0; i < e / 2; i++) m *= p;
    }
    if (rem > 1) { primes[np++] = rem; b *= rem; }
    bool has2 = false;
    for (int i = 0; i < np; i++) if (primes[i] == 2) has2 = true;
    if (!has2) primes[np++] = 2;  /* primes dividing 2n */

    __int128 num, den;
    __int128 m3 = (__int128)m * m * m;
    if (b == 1) {
        num = 2 * m3; den = 3;
        for (int i = 0; i < np; i++) {
            long long p = primes[i];
            num *= (__int128)(p * p - 1);
            den *= (__int128)(p * p);
        }
    } else if ((b & 3) == 1) {
        long long F = b;
        __int128 S = 0;
        for (long long r = 1; r < F; r++) {
            int c = jac(r, b);
            if (c) S += (__int128)c * r * r;
        }
        num = 4 * S * m3; den = F;
        for (int i = 0; i < np; i++) {
            long long p = primes[i];
            if (b % p == 0) continue;
            num *= (__int128)(p * p - jac(p, b));
            den *= (__int128)(p * p);
        }
    } else {
        long long F = 4 * b;
        __int128 S = 0;
        for (long long r = 1; r < F; r += 2) {
            int c = jac(b, r);
            if (c) S += (__int128)c * r * r;
        }
        num = S * m3; den = 2 * (__int128)F;
        for (int i = 0; i < np; i++) {
            long long p = primes[i];
            if (p == 2 || b % p == 0) continue;
            num *= (__int128)(p * p - jac(b, p));
            den *= (__int128)(p * p);
        }
    }
    __int128 v = num / den;
    return (seq_t)(unsigned long long)v;
}
