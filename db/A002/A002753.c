/*
 * A002753  Coefficients of elliptic function sn.
 * https://oeis.org/A002753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [m^floor(n/2)] Y_n(m), where sum_n Y_n(m)
 *            u^(2n+1)/(2n+1)! = -i*sn(i*u, k), m = k^2. Y_n(m) is computed as
 *            the coefficient of y^1 in P_n(y), using P_0(y) = y and P_{n+1} =
 *            P_n''(y)*(1+y^2)*(1+m*y^2) + P_n'(y)*((1+m)*y + 2*m*y^3). This
 *            comes from y'' = (1+m)*y + 2*m*y^3 and (y')^2 = (1+y^2)...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t N = n, E = n / 2, W = E + 1, I = N + 1;
    size_t sz = (size_t)(I * W), t;
    seq_t *cur = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *nx = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t res;
    idx_t s, i, e;

    cur[0] = 1;
    for (s = 1; s <= N; s++) {
        idx_t L = (s < N - s) ? s : N - s;
        idx_t Lp = (s - 1 < N - s + 1) ? s - 1 : N - s + 1;
        idx_t emaxn = (s < E) ? s : E;
        idx_t emaxp = (s - 1 < E) ? s - 1 : E;
        for (i = 0; i <= L; i++)
            for (e = 0; e <= emaxn; e++)
                nx[i * W + e] = 0;
        for (i = 0; i <= Lp; i++) {
            uint64_t j = 2 * (uint64_t)i + 1;
            for (e = 0; e <= emaxp; e++) {
                seq_t v = cur[i * W + e];
                if (v == 0) continue;
                if (i <= L) {
                    seq_t w = v * (seq_t)(j * j);
                    nx[i * W + e] += w;
                    if (e + 1 <= E) nx[i * W + e + 1] += w;
                }
                if (i >= 1 && i - 1 <= L)
                    nx[(i - 1) * W + e] += v * (seq_t)(j * (j - 1));
                if (i + 1 <= L && e + 1 <= E)
                    nx[(i + 1) * W + e + 1] += v * (seq_t)(j * (j + 1));
            }
        }
        {
            seq_t *tmp = cur; cur = nx; nx = tmp;
        }
    }
    res = cur[E];
    for (t = 0; t < sz; t++) { cur[t] = 0; nx[t] = 0; }
    free(cur);
    free(nx);
    return res;
}
