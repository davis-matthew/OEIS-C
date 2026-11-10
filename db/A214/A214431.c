/*
 * A214431  E.g.f. satisfies: A(x) = x + arcsin( A(x) )^2.
 * https://oeis.org/A214431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n![x^n] sin(B(x)), where B(x)=arcsin(A(x)) is the
 *            reversion of sin(y)-y^2; B'(x)*(cos(B)-2B)=1, computed with
 *            integer binomial convolutions on e.g.f. coefficients
 *            (b_n=1,2,13,140,...)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    size_t W = N + 1;
    seq_t *C = mem_new(seq_t, W * W);
    seq_t *b = mem_new(seq_t, N + 2);
    seq_t *s = mem_new(seq_t, N + 2);
    seq_t *d = mem_new(seq_t, N + 2);
    seq_t *q = mem_new(seq_t, N + 2);
    size_t i, k, m;
    seq_t result;

    for (i = 0; i <= N; i++) {
        C[i * W] = 1;
        C[i * W + i] = 1;
        for (k = 1; k < i; k++)
            C[i * W + k] = C[(i - 1) * W + k - 1] + C[(i - 1) * W + k];
    }

    b[1] = 1;
    for (m = 1; m <= N; m++) {
        seq_t sum = 0, sum2 = 0;
        for (k = 1; k < m; k++)
            sum += C[(m - 1) * W + k] * s[k] * b[m - k];
        d[m] = sum;
        for (k = 1; k < m; k++)
            sum2 += C[(m - 1) * W + k] * d[k] * b[m - k];
        s[m] = b[m] - sum2;
        q[m] = b[m] * 2 + d[m];
        if (m < N) {
            seq_t t = 0;
            for (k = 0; k < m; k++)
                t += C[m * W + k] * b[k + 1] * q[m - k];
            b[m + 1] = t;
        }
    }
    result = s[N];

    mem_free(C);
    mem_free(b);
    mem_free(s);
    mem_free(d);
    mem_free(q);
    return result;
}
