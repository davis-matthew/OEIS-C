/*
 * A000281  Expansion of cos(x)/cos(2x).
 * https://oeis.org/A000281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = S(2n), where S = A001586 (Springer numbers), e.g.f.
 *            1/(cos(x)-sin(x)). The m-th derivative of f = 1/(cos x - sin x)
 *            is f*P_m(g) with g = tan(x+Pi/4): P_0 = 1, P_{m+1}(g) = g*P_m(g)
 *            + (1+g^2)*P_m'(g), so S(m) = P_m(1). With coefficients,
 *            [g^j]P_{m+1} = j*[g^(j-1)]P_m + (j+1)*[g^(j+1)]P_m
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t m = 2 * n;
    idx_t s, j, k;
    seq_t *c = (seq_t *)calloc((size_t)(m + 3), sizeof(seq_t));
    seq_t *d = (seq_t *)calloc((size_t)(m + 3), sizeof(seq_t));
    seq_t *t;
    seq_t r;

    c[0] = 1;
    for (s = 0; s < m; s++) {
        /* c holds P_s (nonzero only at indices with parity of s);
           build P_{s+1} in d (indices with parity of s+1) */
        for (j = (s + 1) & 1; j <= s + 1; j += 2) {
            seq_t v = 0;
            if (j >= 1)
                v += (seq_t)j * c[j - 1];
            if (j + 1 <= s)
                v += (seq_t)(j + 1) * c[j + 1];
            d[j] = v;
        }
        t = c; c = d; d = t;
    }

    r = 0;
    for (k = 0; k <= m; k += 2)
        r += c[k];

    for (k = 0; k < m + 3; k++) {
        c[k] = 0;
        d[k] = 0;
    }
    free(c);
    free(d);
    return r;
}
