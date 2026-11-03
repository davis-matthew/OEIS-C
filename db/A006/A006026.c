/*
 * A006026  Number of column-convex polyominoes with perimeter n.
 * https://oeis.org/A006026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^(n+1)] A(x), where A = x - 2x*A + 3A^2 - A^3 (A^3 -
 *            3A^2 + (1+2x)A - x = 0). By Lagrange inversion, A = x*phi(A)
 *            with phi(t) = (1-2t)/(1-3t+t^2), so a(n) = f_n/(n+1), where f_j
 *            = [t^j] phi(t)^(n+1). With m = n+1, f_0 = 1, f_1 = m, and (j+1)
 *            f_{j+1} = (5j+m) f_j - (7(j-1)+2m) f_{j-1} + (...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t a_conv(idx_t m) {
    seq_t *c = mem_new(seq_t, (size_t)(m + 1));
    seq_t *s2 = mem_new(seq_t, (size_t)(m + 1));
    idx_t k, i;
    seq_t r;
    for (k = 0; k <= m; k++) { c[k] = 0; s2[k] = 0; }
    c[1] = 1;
    for (k = 2; k <= m; k++) {
        seq_t t2 = 0, t3 = 0;
        for (i = 1; i < k; i++) t2 += c[i] * c[k - i];
        s2[k] = t2;
        for (i = 1; i + 2 <= k; i++) t3 += c[i] * s2[k - i];
        c[k] = 3 * t2 - 2 * c[k - 1] - t3;
    }
    r = c[m];
    mem_free(c);
    mem_free(s2);
    return r;
}

seq_t a(idx_t n) {
    idx_t m = n + 1;
    idx_t j;
    seq_t fm2, fm1, fj;
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return a_conv(m);
    fm2 = 0;
    fm1 = 1;          /* f_0 */
    fj = (seq_t)m;    /* f_1 */
    for (j = 1; j < n; j++) {
        seq_t num = (seq_t)(5 * j + m) * fj;
        seq_t nx;
        if (j >= 2) num += (seq_t)(2 * j - 4 + 2 * m) * fm2;
        num -= (seq_t)(7 * j - 7 + 2 * m) * fm1;
        nx = num / (seq_t)(j + 1);
        fm2 = fm1;
        fm1 = fj;
        fj = nx;
    }
    return fj / (seq_t)m;
}
