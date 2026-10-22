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
 * Helpers:   a_conv, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/a_conv.h"

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
