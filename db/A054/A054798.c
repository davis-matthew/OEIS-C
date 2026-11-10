/*
 * A054798  Let N(k) and D(k) be the sequences defined in A054765 and A012244;
 *          write N(k)* D(k+j ) - N(k+j)*D(k) = (-1)^(k+1)*(k!)^2*P(k) where
 *          P(k) is a polynomial in k of degree j-1; sequence gives
 *          coefficients of expansion of P(k) in powers of k for j=1,2,3,...
 * https://oeis.org/A054798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   P_j(k) = (2k+2j-1)*P_{j-1}(k) + (k+j-1)^2*P_{j-2}(k), P_0 = 0,
 *            P_1 = 1; row j (j = 1, 2, 3, ...) lists the coefficients of
 *            P_j(k) from k^(j-1) down to k^0. The recurrence follows from
 *            x(m) = (2m-1)*x(m-1) + (m-1)^2*x(m-2), which both N and D
 *            satisfy, applied to W_j(k) = N(k)*D(k+j) - N(k+j)*D(k),...
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 52, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* locate row j and position i (1..j) */
    idx_t j = 1;
    idx_t rem = n;
    while (rem > j) { rem -= j; ++j; }
    size_t J = (size_t)j;
    size_t i = (size_t)rem;
    size_t power = J - i;            /* coefficient of k^power */

    size_t len = J + 3;
    seq_t *prev = mem_new(seq_t, len);   /* P_{t-2} */
    seq_t *cur  = mem_new(seq_t, len);   /* P_{t-1} */
    seq_t *nxt  = mem_new(seq_t, len);
    cur[0] = (seq_t)1;                   /* P_1 = 1, P_0 = 0 */

    for (size_t t = 2; t <= J; ++t) {
        seq_t c  = (seq_t)(2 * t - 1);           /* 2k + (2t-1) */
        seq_t m  = (seq_t)(t - 1);               /* (k + m)^2   */
        seq_t m2 = m * m;
        seq_t tm = m * 2;
        for (size_t p = 0; p <= t; ++p) {
            seq_t v = c * cur[p] + m2 * prev[p];
            if (p >= 1) v += cur[p - 1] * 2 + tm * prev[p - 1];
            if (p >= 2) v += prev[p - 2];
            nxt[p] = v;
        }
        seq_t *tmp = prev; prev = cur; cur = nxt; nxt = tmp;
    }

    seq_t res = cur[power];
    mem_free(prev);
    mem_free(cur);
    mem_free(nxt);
    return res;
}
