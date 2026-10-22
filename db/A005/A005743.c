/*
 * A005743  Number of fanout-free Boolean functions of n variables using And,
 *          Or, Not and Majority gates.
 * https://oeis.org/A005743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)*A005615(k), with A005615(0) =
 *            2 and A005615(k) = 2*c(k) for k >= 1. Here c(k) counts
 *            complement-classes of read-once {And,Or,Not,Maj} functions of k
 *            variables, with these EGFs: T = x + (2/3)*C^3 (a variable, or
 *            Maj of 3 classes; 4 classes per triple) U = 2*T + A...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    idx_t L = n + 1, m, j, k;
    seq_t *c = alloc_zero(L);
    seq_t *u = alloc_zero(L);
    seq_t *E = alloc_zero(L);
    seq_t *P = alloc_zero(L);
    seq_t *bin = alloc_zero(L);
    seq_t res, b;

    E[0] = 1;
    for (m = 1; m <= n; m++) {
        seq_t mm = 0, aa = 0, pp = 0, t;
        /* bin[j] = binomial(m-1, j) */
        bin[0] = 1;
        for (j = 1; j < m; j++)
            bin[j] = bin[j - 1] * (seq_t)(m - j) / (seq_t)j;
        for (k = 1; k < m; k++) {
            mm += bin[k - 1] * c[k] * P[m - k];
            aa += bin[k - 1] * u[k] * E[m - k];
            pp += bin[k - 1] * c[k] * c[m - k];
        }
        t = mm * 4;
        if (m == 1) t += 1;
        c[m] = t + aa;
        u[m] = t * 2 + aa;
        E[m] = u[m] + aa;
        P[m] = pp;
    }

    res = 2;
    b = 1;
    for (k = 1; k <= n; k++) {
        b = b * (seq_t)(n - k + 1) / (seq_t)k;
        res += b * c[k] * 2;
    }

    release(c, L);
    release(u, L);
    release(E, L);
    release(P, L);
    release(bin, L);
    return res;
}
