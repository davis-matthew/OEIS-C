/*
 * A001507  a(n) is the number of c-nets with n+1 vertices and 2n+1 edges, n
 *          >= 1.
 * https://oeis.org/A001507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A290326(n+1,n) = 4*S2 - S1, from the Mullin-Schellenberg
 *            c-net generating function (Lagrange inversion), where S1 =
 *            Sum_{l=0..n} (-1)^l*C(l+2,2)*C(2n,n-l)*C(2n-1-l,n-1), S2 =
 *            Sum_{l=0..n-1} (-1)^l*C(l+2,2)*C(2n-1,n-1-l)*C(2n-2-l,n-2);
 *            a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   a1507_bn, a1507_conv, a1507_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1507_bn.h"
#include "../utils/a1507_conv.h"
#include "../utils/a1507_fixed.h"

seq_t a(idx_t n)
{
    idx_t l, i;
    if (n < 4) return 0;
    if (a1507_fixed()) {
        unsigned __int128 P1 = 0, N1 = 0, P2 = 0, N2 = 0, t, r;
        if (n > 60) return 0;
        for (l = 0; l <= n; l++) {
            t = (unsigned __int128)((l + 1) * (l + 2) / 2)
                * a1507_bn(2 * n, n - l) * a1507_bn(2 * n - 1 - l, n - 1);
            if (l % 2 == 0) P1 += t; else N1 += t;
        }
        for (l = 0; l + 1 <= n; l++) {
            t = (unsigned __int128)((l + 1) * (l + 2) / 2)
                * a1507_bn(2 * n - 1, n - 1 - l) * a1507_bn(2 * n - 2 - l, n - 2);
            if (l % 2 == 0) P2 += t; else N2 += t;
        }
        r = 4 * P2 + N1 - 4 * N2 - P1;
        return a1507_conv(r);
    } else {
        seq_t X = 1, W = 1, T, Z, P1 = 0, N1 = 0, P2 = 0, N2 = 0, term;
        for (i = 0; i < n; i++) { X = X * (seq_t)(2 * n - i); X = X / (seq_t)(i + 1); }
        T = X * (X / 2);              /* C(2n,n)*C(2n-1,n-1) */
        for (i = 0; i < n - 2; i++) { W = W * (seq_t)(2 * n - 2 - i); W = W / (seq_t)(i + 1); }
        Z = (X / 2) * W;              /* C(2n-1,n-1)*C(2n-2,n-2) */
        for (l = 0; l <= n; l++) {
            term = T * (seq_t)((l + 1) * (l + 2) / 2);
            if (l % 2 == 0) P1 += term; else N1 += term;
            if (l < n) {
                T = T * (seq_t)((n - l) * (n - l));
                T = T / (seq_t)((n + l + 1) * (2 * n - 1 - l));
            }
        }
        for (l = 0; l + 1 <= n; l++) {
            term = Z * (seq_t)((l + 1) * (l + 2) / 2);
            if (l % 2 == 0) P2 += term; else N2 += term;
            if (l + 1 < n) {
                Z = Z * (seq_t)((n - 1 - l) * (n - l));
                Z = Z / (seq_t)((n + l + 1) * (2 * n - 2 - l));
            }
        }
        return (P2 * 4 + N1) - (N2 * 4 + P1);
    }
}
