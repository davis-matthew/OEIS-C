/*
 * A000488  Generalized tangent numbers d_(n,3).
 * https://oeis.org/A000488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n,3) = 5! * (2n/Pi)^6 * n^(-1/2) * Sum_{m odd >= 1}
 *            (n/m) m^(-6), where (n/m) is the Jacobi symbol. Write n = f^2*q
 *            with q squarefree. Then a(n) = D(q) * f^5 * Prod_{p^e || f}
 *            (p^(6e) - (q/p) p^(6e-6)), using (q/2) = 0, and D(1) = 8 (a(1) =
 *            16 is the tangent number); for q = 2,3 (mod 4):...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 38, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a488_base, a488_jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a488_base.h"
#include "../utils/a488_jacobi.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n, q = 1, p;
    uint64_t fp[64]; int fe[64]; int nf = 0, i, j;
    __int128 res;
    uint64_t lo, hi;
    seq_t x;

    if (n == 1) return (seq_t)16;
    for (p = 2; p * p <= m; p++) {
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        if (e) {
            if (e & 1) q *= p;
            if (e / 2) { fp[nf] = p; fe[nf] = e / 2; nf++; }
        }
    }
    if (m > 1) q *= m;

    res = a488_base(q);
    for (i = 0; i < nf; i++) {
        __int128 pp = (__int128)fp[i], f5 = 1, p6 = 1, term;
        for (j = 0; j < 5 * fe[i]; j++) f5 *= pp;
        for (j = 0; j < 6 * (fe[i] - 1); j++) p6 *= pp;
        term = p6 * pp * pp * pp * pp * pp * pp;
        if (fp[i] != 2) term -= (__int128)a488_jacobi(q, fp[i]) * p6;
        res = res * f5 * term;
    }

    lo = (uint64_t)res;
    hi = (uint64_t)(res >> 64);
    if (hi == 0) return (seq_t)lo;
    x = (seq_t)hi;
    for (i = 0; i < 64; i++) x = x + x;
    x += (seq_t)lo;
    return x;
}
