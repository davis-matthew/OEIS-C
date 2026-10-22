/*
 * A002432  Denominators of zeta(2*n)/Pi^(2*n).
 * https://oeis.org/A002432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, frac
 * Formula:   a(n) = denominator(|B_{2n}|*2^(2n-1)/(2n)!), a(0) = 2, built one
 *            prime at a time: v_2(a(n)) = 1 if n is a power of 2, else 0; for
 *            odd p with (p-1) | 2n: v_p(a(n)) = v_p((2n)!) + 1 (von
 *            Staudt-Clausen); for other odd p <= 2n: v_p(a(n)) = max(0,
 *            v_p((2n)!) - v_p(B_{2n})), where v_p(B_{2n}) = v_p(T_...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   a2432_fixed, a2432_isprime, a2432_legendre, a2432_mulmod,
 *            a2432_powmod, a2432_tan_mod, a2432_val_mod, a2432_vp,
 *            alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/a2432_fixed.h"
#include "../utils/a2432_isprime.h"
#include "../utils/a2432_legendre.h"
#include "../utils/a2432_mulmod.h"
#include "../utils/a2432_powmod.h"
#include "../utils/a2432_tan_mod.h"
#include "../utils/a2432_val_mod.h"
#include "../utils/a2432_vp.h"

/* exact tangent numbers T_1..T_cap, big mode only */
static seq_t *a2432_T = NULL;
static idx_t a2432_cap = 0;

static void a2432_ensure(idx_t N) {
    idx_t newN, j, k;
    if (N <= a2432_cap) return;
    newN = a2432_cap + a2432_cap / 2;
    if (newN < N) newN = N;
    if (!a2432_T) a2432_T = alloc_zero(newN + 1);
    else a2432_T = grow_zero(a2432_T, a2432_cap + 1, newN + 1);
    a2432_T[1] = 1;
    for (k = 2; k <= newN; k++) a2432_T[k] = a2432_T[k - 1] * (seq_t)(k - 1);
    for (k = 2; k <= newN; k++)
        for (j = k; j <= newN; j++)
            a2432_T[j] = a2432_T[j - 1] * (seq_t)(j - k) + a2432_T[j] * (seq_t)(j - k + 2);
    a2432_cap = newN;
}

seq_t a(idx_t n) {
    int fixed;
    uint64_t m2, p, i;
    seq_t res = 1;
    seq_t Q = 0;
    if (n == 0) return (seq_t)2;
    fixed = a2432_fixed();
    if (fixed && n > 150) return (seq_t)0; /* cannot fit in a fixed-width type */
    m2 = 2 * (uint64_t)n;
    if ((n & (n - 1)) == 0) res *= 2;
    if (!fixed) {
        a2432_ensure(n);
        Q = ((seq_t)1 << (2 * n)) - 1;
    }
    for (p = 3; p <= m2 + 1; p += 2) {
        uint64_t f, e;
        if (!a2432_isprime(p)) continue;
        f = a2432_legendre(m2, p);
        if (m2 % (p - 1) == 0) {
            e = f + 1;
        } else {
            long long vB;
            uint64_t u = a2432_vp(m2, p);
            if (f == 0) continue;
            if (fixed) {
                uint64_t pk = 1, K = 0, vT, vL, L;
                while (pk <= ((uint64_t)1 << 62) / p) { pk *= p; K++; }
                vT = a2432_val_mod(a2432_tan_mod(n, pk), p, K);
                L = (a2432_powmod(4, n, pk) + pk - 1) % pk;
                vL = a2432_val_mod(L, p, K);
                if (vT >= K) vB = (long long)(f + 1);
                else vB = (long long)vT + (long long)u - (long long)vL;
            } else {
                seq_t t, q;
                seq_t sp = (seq_t)p;
                uint64_t vL = 0, vT = 0, cap;
                q = Q;
                while (q % sp == 0) { q /= sp; vL++; }
                cap = f + u + vL;
                t = a2432_T[n];
                while (vT < cap && t % sp == 0) { t /= sp; vT++; }
                vB = (long long)vT + (long long)u - (long long)vL;
            }
            e = (vB < (long long)f) ? (uint64_t)((long long)f - vB) : 0;
        }
        for (i = 0; i < e; i++) res *= (seq_t)p;
    }
    return res;
}
