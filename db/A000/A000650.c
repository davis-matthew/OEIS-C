/*
 * A000650  Number of alkyls S C_{n+4} H_{2n+4} with n carbon atoms.
 * https://oeis.org/A000650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: A(x) = (r(x)^4 + r(x^2)^2)/2, r(x) = A000598(x), where
 *            r(x) = 1 + x*(r(x)^3 + 3*r(x)*r(x^2) + 2*r(x^3))/6. With s =
 *            r^2: a(n) = Sum_{0<=i<n/2} s(i)*s(n-i) + [n
 *            even]*s(n/2)*(s(n/2)+1)/2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *R = 0;   /* A000598 coefficients */
static seq_t *S = 0;   /* coefficients of r(x)^2 */
static idx_t have = 0; /* number of computed entries */
static idx_t cap = 0;

static void extend(idx_t upto) {
    if (upto + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < upto + 1) nc *= 2;
        R = a306_grow(R, cap, nc);
        S = a306_grow(S, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        R[0] = 1;
        S[0] = 1;
        have = 1;
    }
    while (have <= upto) {
        idx_t m = have;
        idx_t k = m - 1;
        seq_t t3 = 0, p = 0, t;
        idx_t i, j;
        for (i = 0; i <= k; i++) t3 += S[i] * R[k - i];
        for (j = 0; 2 * j <= k; j++) p += R[j] * R[k - 2 * j];
        t = t3 + 3 * p;
        if (k % 3 == 0) t += 2 * R[k / 3];
        R[m] = t / 6;
        {
            seq_t s = 0;
            for (i = 0; i <= m; i++) s += R[i] * R[m - i];
            S[m] = s;
        }
        have = m + 1;
    }
}

seq_t a(idx_t n) {
    seq_t res = 0;
    idx_t i;
    extend(n);
    for (i = 0; 2 * i < n; i++) res += S[i] * S[n - i];
    if (n % 2 == 0) {
        seq_t m = S[n / 2];
        if (m % 2 == 0) res += (m / 2) * (m + 1);
        else res += m * ((m + 1) / 2);
    }
    return res;
}
