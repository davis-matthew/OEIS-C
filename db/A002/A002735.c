/*
 * A002735  Related to Euler numbers, expansion of e.g.f. sec(x)*tan^2(x).
 * https://oeis.org/A002735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(2,n) where b(m,1) = m^2, b(m,2*k) = Sum_{j=1..m+1}
 *            b(j,2*k-1), b(m,2*k+1) = m^2 * b(m,2*k); a(2*k+1) = 4*a(2*k),
 *            a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N, R1, m, lev;
    seq_t *v;
    seq_t s, result;

    if (n <= 1) return (seq_t)4;

    N = (n % 2 == 0) ? n : n - 1;      /* even level to compute b(2,N) */
    R1 = N / 2 + 2;                     /* range of m needed at level 1 */

    v = (seq_t *)calloc((size_t)(R1 + 3), sizeof(seq_t));
    for (m = 1; m <= R1; m++)
        v[m] = (seq_t)m * (seq_t)m;

    for (lev = 2; lev <= N; lev++) {
        idx_t R = 2 + N / 2 - lev / 2;  /* range of m needed at this level */
        if (lev % 2 == 0) {
            s = v[1];
            for (m = 1; m <= R; m++) {
                s += v[m + 1];
                v[m] = s;
            }
        } else {
            for (m = 1; m <= R; m++)
                v[m] = (seq_t)m * (seq_t)m * v[m];
        }
    }

    result = v[2];
    free(v);
    if (n % 2 == 1) result *= 4;
    return result;
}
