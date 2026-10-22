/*
 * A006135  T(n+2,2) from table A045912 of characteristic polynomial of
 *          negative Pascal matrix.
 * https://oeis.org/A006135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n+2,2) = e_2 of the symmetric Pascal matrix P of order
 *            m = n+2, where P[i][j] = binomial(i+j,i) for 0 <= i,j < m.
 *            Equivalently, a(n) = Sum_{0<=i<j<m}
 *            (binomial(2i,i)*binomial(2j,j) - binomial(i+j,i)^2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t m = n + 2;
    idx_t L = 2 * m - 1;              /* Pascal rows 0 .. 2m-2 */
    seq_t *row = alloc_zero(L);
    seq_t A = 0, B = 0, T = 0;
    idx_t s, i;
    row[0] = 1;
    for (s = 0; s < L; s++) {
        if (s > 0)
            for (i = s; i >= 1; i--)
                row[i] += row[i - 1];
        if (s % 2 == 0) {
            /* diagonal entry binomial(2j,j), j = s/2 */
            seq_t c = row[s / 2];
            A += c * T;               /* Sum_{i<j} C(2i,i)*C(2j,j) */
            T += c;
        }
        /* pairs i < j, i + j = s, j < m */
        idx_t lo = (s + 1 > m) ? s + 1 - m : 0;
        for (i = lo; 2 * i < s; i++)
            B += row[i] * row[i];
    }
    release(row, L);
    return A - B;
}
