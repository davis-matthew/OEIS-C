/*
 * A006573  Denominators of an asymptotic expansion for the number of forests
 *          on n nodes (A001858).
 * https://oeis.org/A006573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of C(n-1), where A001858(n) ~
 *            sqrt(e)*n^(n-1)*Sum_{r>=0} C(r)/n^r, C(r) = (-1)^r *
 *            Sum_{q=1..r} d(r+q,q) * h(r+q), d = associated Stirling numbers
 *            of the first kind (A008306), h(2m+1) = h(2m+2) = -(-1/2)^m/m!;
 *            d(r+q,q) via d(n,k) = (n-1)*(d(n-1,k) + d(n-2,k-1)); a(0) = a(1)
 *            = 1...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   a6573_small_prime, alloc_zero, gcd_seq, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_seq.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_big.h"
#include "../utils/a6573_small_prime.h"

typedef unsigned __int128 u128;

static u128 a6573_addmod(u128 x, u128 y, u128 m)
{
    u128 s = x + y;
    if (s >= m) s -= m;
    return s;
}

static u128 a6573_mulmod(u128 x, u128 y, u128 m)
{
    u128 res = 0;
    x %= m;
    while (y) {
        if (y & 1) res = a6573_addmod(res, x, m);
        x = a6573_addmod(x, x, m);
        y >>= 1;
    }
    return res;
}

static seq_t a6573_big(idx_t r)
{
    idx_t k, q, m;
    seq_t *D = alloc_zero(r + 1);
    seq_t *F = alloc_zero(r);
    seq_t P = 0, Q = 0, N, M, g, res;
    D[0] = 1;
    for (k = 1; k <= r; k++) {
        for (q = k; q >= 1; q--)
            D[q] = (D[q] + D[q - 1]) * (seq_t)(k + q - 1);
        D[0] = 0;
    }
    F[r - 1] = 1;
    for (m = r - 1; m >= 1; m--)
        F[m - 1] = F[m] * (seq_t)(2 * m);
    for (q = 1; q <= r; q++) {
        m = (r + q - 1) / 2;
        if (m & 1) Q += D[q] * F[m];
        else P += D[q] * F[m];
    }
    M = F[0];
    N = (P > Q) ? P - Q : Q - P;
    if (N == 0) res = 1;
    else {
        g = gcd_seq(N, M);
        res = M / g;
    }
    release(D, r + 1);
    release(F, r);
    return res;
}

static seq_t a6573_fixed(idx_t r)
{
    seq_t res = 1;
    uint64_t p;
    idx_t k, q, m, e, E, v;
    u128 *D = (u128 *)malloc((size_t)(r + 1) * sizeof(u128));
    u128 *F = (u128 *)malloc((size_t)r * sizeof(u128));
    for (p = 2; p <= (r - 1 > 2 ? r - 1 : 2); p++) {
        u128 mod, pk, P, Q, x;
        uint64_t t;
        if (!a6573_small_prime(p)) continue;
        E = (p == 2) ? r - 1 : 0;
        for (t = p; t <= r - 1; t *= p) E += (r - 1) / t;
        if (E == 0) continue;
        mod = 1;
        for (e = 0; e < E; e++) mod *= p;
        for (q = 0; q <= r; q++) D[q] = 0;
        D[0] = 1;
        for (k = 1; k <= r; k++) {
            for (q = k; q >= 1; q--)
                D[q] = a6573_mulmod(a6573_addmod(D[q], D[q - 1], mod), (u128)(k + q - 1), mod);
            D[0] = 0;
        }
        F[r - 1] = 1 % mod;
        for (m = r - 1; m >= 1; m--)
            F[m - 1] = a6573_mulmod(F[m], (u128)(2 * m), mod);
        P = 0; Q = 0;
        for (q = 1; q <= r; q++) {
            m = (r + q - 1) / 2;
            if (m & 1) Q = a6573_addmod(Q, a6573_mulmod(D[q], F[m], mod), mod);
            else P = a6573_addmod(P, a6573_mulmod(D[q], F[m], mod), mod);
        }
        x = a6573_addmod(P, mod - Q, mod) % mod;
        v = 0;
        if (x == 0) v = E;
        else
            while (v < E && x % p == 0) { x /= p; v++; }
        for (e = v; e < E; e++) res *= (seq_t)p;
    }
    free(D);
    free(F);
    return res;
}

seq_t a(idx_t n)
{
    idx_t r;
    if (n <= 1) return 1;
    r = n - 1;
    if (seq_is_big()) return a6573_big(r);
    if (r > 60) return 0; /* value far exceeds any fixed-width seq_t */
    return a6573_fixed(r);
}
