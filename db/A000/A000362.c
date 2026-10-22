/*
 * A000362  Generalized class numbers c_(n,2).
 * https://oeis.org/A000362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   write -4n = d*f^2 with d a fundamental discriminant and chi_d(m)
 *            = (d|m) (Kronecker). Then a(n) = -(f^4/5) * B_{5,chi_d} *
 *            Prod_{p^e || f} p^(5(e-1)) * (p^5 - chi_d(p)), where B_{5,chi_d}
 *            = k^4 * Sum_{r=1..k} chi_d(r) * B_5(r/k), k = |d|, B_5(x) = x^5
 *            - 5x^4/2 + 5x^3/3 - x/6. The exceptional term...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 112, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a362_jacobi, a362_kron, a362_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a362_jacobi.h"
#include "../utils/a362_kron.h"
#include "../utils/a362_to_seq.h"

seq_t a(idx_t n)
{
    long long x, s = 1, t = 1, p, d, k, f, r;
    __int128 S = 0, M = 1, K, res;

    if (n == 1) return (seq_t)5;

    /* n = s * t^2 with s squarefree */
    x = (long long)n;
    for (p = 2; p * p <= x; p++) {
        int e = 0;
        while (x % p == 0) { x /= p; e++; }
        if (e & 1) s *= p;
        while (e >= 2) { t *= p; e -= 2; }
    }
    if (x > 1) s *= x;

    if (s % 4 == 3) { d = -s; k = s; f = 2 * t; }
    else { d = -4 * s; k = 4 * s; f = t; }

    /* S = 6k * B_{5,chi_d} */
    K = (__int128)k;
    for (r = 1; r <= k; r++) {
        int c = a362_kron(d, r);
        __int128 R, v;
        if (c == 0) continue;
        R = (__int128)r;
        v = ((6 * R - 15 * K) * R + 10 * K * K) * R * R - K * K * K * K;
        v *= R;
        if (c > 0) S += v; else S -= v;
    }

    /* M = f^4 * prod over p^e || f of p^(5(e-1)) * (p^5 - chi_d(p)) */
    {
        long long y = f;
        __int128 F = (__int128)f;
        M = F * F * F * F;
        for (p = 2; p * p <= y || y > 1; p++) {
            int e = 0;
            if (p * p > y) p = y;
            while (y % p == 0) { y /= p; e++; }
            if (e > 0) {
                __int128 P = (__int128)p;
                __int128 P5 = P * P * P * P * P;
                int i;
                for (i = 1; i < e; i++) M *= P5;
                M *= P5 - (__int128)a362_kron(d, p);
            }
            if (y == 1) break;
        }
    }

    res = (-S * M) / (30 * K);
    if (res < 0) res = -res;
    return a362_to_seq(res);
}
