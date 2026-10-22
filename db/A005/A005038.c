/*
 * A005038  Number of nonequivalent dissections of a polygon into n pentagons
 *          by nonintersecting diagonals up to rotation.
 * https://oeis.org/A005038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   dissymmetry theorem. Let F(x) = 1 + x*F(x)^4, so [x^m]F^r =
 *            r/(4m+r)*C(4m+r,m). a(n) = P(n) + E(n) - Q(n), where P(n) =
 *            ([x^(n-1)]F^5 + 4*[5|(n-1)]*F_((n-1)/5)) / 5 (rooted at a
 *            pentagon), E(n) = ([x^n]F^2 - 2*F_n + [2|n]*F_(n/2)) / 2 (rooted
 *            at a diagonal), Q(n) = [x^(n-1)]F^5 - [x^(n-1)]F^4 (pe...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   a005038_coef, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a005038_coef.h"

typedef unsigned __int128 a005038_u128;

/* Fixed-width path: all arithmetic mod 2^128 (no inexact division). */
static seq_t a005038_fixed(idx_t n)
{
    a005038_u128 F[70], G[70], H[70];
    a005038_u128 inv5, F5, F4, f5, P, E, Q;
    idx_t m, i;
    int it;
    if (n > 64) return 0; /* a(n) >= 2^128 here; cannot fit any fixed width */
    F[0] = G[0] = H[0] = 1;
    for (m = 1; m <= n; m++) {
        F[m] = H[m - 1];
        G[m] = 0;
        for (i = 0; i <= m; i++) G[m] += F[i] * F[m - i];
        H[m] = 0;
        for (i = 0; i <= m; i++) H[m] += G[i] * G[m - i];
    }
    inv5 = 5;
    for (it = 0; it < 7; it++) inv5 *= (a005038_u128)2 - (a005038_u128)5 * inv5;

    F5 = 0;
    for (i = 0; i <= n - 1; i++) F5 += H[i] * F[n - 1 - i];
    F4 = H[n - 1];
    f5 = ((n - 1) % 5 == 0) ? F[(n - 1) / 5] : 0;
    P = (F5 + 4 * f5) * inv5;

    E = 0;
    for (i = 1; i < n - i; i++) E += F[i] * F[n - i];
    if (n % 2 == 0) {
        a005038_u128 fk = F[n / 2]; /* exact: n/2 <= 32 */
        if (fk % 2 == 0) E += (fk / 2) * (fk + 1);
        else E += fk * ((fk + 1) / 2);
    }

    Q = F5 - F4;
    return u128_to_seq(P + E - Q);
}

seq_t a(idx_t n)
{
    seq_t F5, F4, f5, P, F2, Fn, f2, E, Q;
    if (seq_is_fixed_width()) return a005038_fixed(n);

    F5 = a005038_coef(n - 1, 5);
    F4 = a005038_coef(n - 1, 4);
    f5 = ((n - 1) % 5 == 0) ? a005038_coef((n - 1) / 5, 1) : (seq_t)0;
    P = (F5 + f5 * 4) / 5;

    F2 = a005038_coef(n, 2);
    Fn = a005038_coef(n, 1);
    f2 = (n % 2 == 0) ? a005038_coef(n / 2, 1) : (seq_t)0;
    E = (F2 + f2 - Fn * 2) / 2;

    Q = F5 - F4;
    return P + E - Q;
}
