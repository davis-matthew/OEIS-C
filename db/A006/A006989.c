/*
 * A006989  Log of g.f. of numbers of preferential arrangements.
 * https://oeis.org/A006989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. of a(n) shifted: sum_{n>=0} a(n) x^(n+1)/(n+1)! = log(1 +
 *            integral(B)), B = e.g.f. of A000670 (b(0) = 1, b(m) =
 *            Sum_{k=0..m-1} C(m,k) b(k)). With g(0) = 1, g(m+1) = b(m) and f
 *            = log(G): f(m+1) = g(m+1) - Sum_{k=0..m-1} C(m,k) f(k+1) g(m-k),
 *            and a(n) = f(n+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    seq_t *b = alloc_zero(n + 1);    /* Fubini numbers b[0..n] */
    seq_t *f = alloc_zero(n + 2);    /* log coefficients f[1..n+1] */
    seq_t *row = alloc_zero(n + 2);  /* Pascal row C(m, .) */
    row[0] = (seq_t)1;

    for (idx_t m = 0; m <= n; m++) {
        if (m == 0) {
            b[0] = (seq_t)1;
        } else {
            seq_t s = (seq_t)0;
            for (idx_t k = 0; k < m; k++)
                s += row[k] * b[k];
            b[m] = s;
        }
        /* g(m+1) = b[m], g(j) = b[j-1] for j >= 1 */
        seq_t s = b[m];
        for (idx_t k = 0; k < m; k++)
            s -= row[k] * f[k + 1] * b[m - k - 1];
        f[m + 1] = s;

        /* advance Pascal row from C(m,.) to C(m+1,.) */
        row[m + 1] = (seq_t)1;
        for (idx_t k = m; k >= 1; k--)
            row[k] += row[k - 1];
    }

    seq_t result = f[n + 1];
    release(b, n + 1);
    release(f, n + 2);
    release(row, n + 2);
    return result;
}
