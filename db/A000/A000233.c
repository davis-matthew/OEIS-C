/*
 * A000233  Generalized class numbers c_(n,1).
 * https://oeis.org/A000233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n > 1 write n = a'*s^2 with a' squarefree, and let chi =
 *            Kronecker character of D = -a' (if a' == 3 mod 4) or D = -4a'
 *            (otherwise), f = |D|. B_{3,chi} = (1/(2f)) * Sum_{r=1..f}
 *            chi(r)*(2r^3 - 3f r^2 + f^2 r). L_n(3) = L(3,chi) * Prod_{p |
 *            2s} (1 - chi(p)/p^3), and L(3,chi) = 2 Pi^3 sqrt(f) B_...
 * Range:     u8 n <= 11, u16 n <= 109, u32 n <= 9118, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a233_jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a233_jacobi.h"

static int a233_case1;
static unsigned long long a233_ap;

static int a233_chi(unsigned long long r)
{
    if (a233_case1)
        return a233_jacobi(r % a233_ap, a233_ap);
    if ((r & 1) == 0) return 0;
    return a233_jacobi((r - a233_ap % r) % r, r);
}

seq_t a(idx_t n)
{
    unsigned long long t = n, ap = 1, s = 1, p, f, r;
    unsigned long long sp[64];
    int ns = 0, i;
    __int128 T = 0, s5, Q = 1, num, c, P2, fi;

    if (n == 1) return (seq_t)1;

    for (p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            int e = 0, k;
            while (t % p == 0) { t /= p; e++; }
            if (e & 1) ap *= p;
            if (e >= 2) sp[ns++] = p;
            for (k = 0; k < e / 2; k++) s *= p;
        }
    }
    ap *= t;

    a233_ap = ap;
    a233_case1 = (ap % 4 == 3);
    f = a233_case1 ? ap : 4 * ap;
    fi = (__int128)f;

    for (r = 1; r <= f; r++) {
        int ch = a233_chi(r);
        if (ch != 0) {
            __int128 R = (__int128)r;
            __int128 term = 2 * R * R * R - 3 * fi * R * R + fi * fi * R;
            if (ch > 0) T += term; else T -= term;
        }
    }

    s5 = (__int128)s * s * s * s * s;
    for (i = 0; i < ns; i++) {
        __int128 p3 = (__int128)sp[i] * sp[i] * sp[i];
        s5 /= p3;
        Q *= p3 - a233_chi(sp[i]);
    }
    if (s & 1) {
        Q *= 8 - a233_chi(2);
        P2 = 8;
    } else {
        P2 = 1;
    }

    num = s5 * T * Q;
    if (a233_case1)
        c = 16 * num / (3 * (__int128)ap * P2);
    else
        c = num / (24 * (__int128)ap * P2);
    if (c < 0) c = -c;

    return (seq_t)(unsigned long long)c;
}
