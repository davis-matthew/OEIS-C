/*
 * A005037  Number of nonequivalent dissections of a polygon into n pentagons
 *          by nonintersecting diagonals rooted at a cell up to rotation.
 * https://oeis.org/A005037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [z^(n-1)] (4*G(z^5) + G(z)^5)/5, G(z) = 1 + z*G(z)^4
 *            (A002293). With m = n-1: [z^m] G^5 = 5*(4m+4)!/(m!*(3m+5)!),
 *            [z^k] G = (4k)!/(k!*(3k+1)!), so a(n) = (5*(4m+4)!/(m!*(3m+5)!)
 *            + 4*[5|m]*(4k)!/(k!*(3k+1)!))/5, k = m/5. The factorial
 *            quotients are built from prime exponents (Legendre's for...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   fact_quot, leg_exp, seq_is_fixed_width, spow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/fact_quot.h"
#include "../utils/leg_exp.h"
#include "../utils/spow_seq.h"

seq_t a(idx_t n) {
    idx_t m = n - 1;
    idx_t lim = 4 * m + 5;
    char *comp = (char *)malloc((size_t)lim + 1);
    memset(comp, 0, (size_t)lim + 1);
    for (idx_t i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= lim; j += i) comp[j] = 1;

    seq_t X = fact_quot(comp, lim, 4 * m + 4, m, 3 * m + 5, 1);
    seq_t g = 0;
    if (m % 5 == 0) {
        idx_t k = m / 5;
        g = fact_quot(comp, lim, 4 * k, k, 3 * k + 1, 0);
    }
    free(comp);

    seq_t s = X + g * 4;
    if (seq_is_fixed_width()) {
        /* exact division by 5 modulo 2^w: multiply by 5^{-1} = 0xCC..CCD */
        seq_t inv = 0;
        for (int i = 0; i < 16; i++) inv = (seq_t)((inv << 8) | 0xCC);
        inv = inv + 1;
        return s * inv;
    }
    return s / 5;
}
