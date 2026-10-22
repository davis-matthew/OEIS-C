/*
 * A000259  Number of certain rooted planar maps.
 * https://oeis.org/A000259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k-1)*F(k-2)*(C(3n-1,n-k) -
 *            2*C(3n-1,n-k-1)), using k/n*C(3n,n-k) = C(3n-1,n-k) -
 *            2*C(3n-1,n-k-1); F(-1) = 1, F(0) = 0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   fixed_width_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/fixed_width_seq.h"

seq_t a(idx_t n)
{
    idx_t m = 3 * n - 1, j, k;
    seq_t *C = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    seq_t pos = 0, neg = 0, fa = 1, fb = 0, res;

    if (fixed_width_seq()) {
        /* Pascal's triangle, truncated to columns 0..n (exact mod 2^w) */
        idx_t r, top;
        for (j = 0; j <= n; j++) C[j] = 0;
        C[0] = 1;
        for (r = 1; r <= m; r++) {
            top = r < n ? r : n;
            for (j = top; j >= 1; j--) C[j] += C[j - 1];
        }
    } else {
        C[0] = 1;
        for (j = 0; j < n; j++) {
            seq_t c = C[j];
            c *= (seq_t)(m - j);
            c /= (seq_t)(j + 1);
            C[j + 1] = c;
        }
    }

    for (k = 1; k <= n; k++) {
        seq_t f = fa, t;
        seq_t p = f * C[n - k];
        seq_t q = 0;
        if (n - k >= 1) q = f * C[n - k - 1] * 2;
        if (k & 1) { pos += p; neg += q; }
        else       { neg += p; pos += q; }
        t = fa + fb;
        fa = fb;
        fb = t;
    }
    free(C);
    res = pos - neg;
    return res;
}
