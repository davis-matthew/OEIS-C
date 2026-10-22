/*
 * A003082  Number of multigraphs with 4 nodes and n edges.
 * https://oeis.org/A003082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   G.f.:
 *            (1+x^3+x^4+x^5+x^6+x^9)/((1-x)*(1-x^2)^2*(1-x^3)^2*(1-x^4)),
 *            which equals the OEIS g.f.
 *            (1-x+x^2+x^4+x^6-x^7+x^8)/((1-x)^6*(1+x)^2*(1+x^2)*(1+x+x^2)^2)
 *            with numerator and denominator multiplied by (1+x)
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 412, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const idx_t parts[6] = {1, 2, 2, 3, 3, 4};
    static const idx_t numexp[6] = {0, 3, 4, 5, 6, 9};
    seq_t *s;
    seq_t res;
    idx_t i;
    int p;

    s = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    for (i = 0; i <= n; i++)
        s[i] = 0;
    s[0] = 1;
    for (p = 0; p < 6; p++) {
        idx_t k = parts[p];
        for (i = k; i <= n; i++)
            s[i] += s[i - k];
    }
    res = 0;
    for (p = 0; p < 6; p++) {
        if (numexp[p] <= n)
            res += s[n - numexp[p]];
    }
    free(s);
    return res;
}
