/*
 * A000901  Number of solutions to the rook problem on a 2n X 2n board having
 *          a certain symmetry group (see Robinson for details).
 * https://oeis.org/A000901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (2^n*n! - F(n) - B(n))/4, where F(n) = n!/(n/2)! for even
 *            n, 0 for odd n (boards fixed by a 90-degree rotation), and B(n)
 *            = 2*B(n-1) + 2*(n-1)*B(n-2), B(0) = 1, B(1) = 2 (A000898, boards
 *            fixed by both diagonal reflections). a(1) = 0. Each value is
 *            carried as 4*q + r, so the division by 4 i...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   a901_mulk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a901_mulk.h"

seq_t a(idx_t n)
{
    seq_t T, qB, qF, q1, q2, qs;
    unsigned rB, rF, r1, r2, rs;
    idx_t i;

    if (n < 2) return (seq_t)0;

    /* T = 2^(n-2) * n! */
    T = 1;
    for (i = 2; i <= n; i++) T *= (seq_t)i;
    for (i = 2; i < n + 0; i++) T *= 2;

    /* B(n) as 4*q + r */
    q2 = 0; r2 = 1; /* B(0) */
    q1 = 0; r1 = 2; /* B(1) */
    for (i = 2; i <= n; i++) {
        idx_t k = i - 1;
        idx_t kq = k / 4;
        unsigned kr = (unsigned)(k % 4);
        unsigned small = 2 * r1 + 2 * kr * r2;
        seq_t nq = q1 * 2 + q2 * (seq_t)k * 2 + (seq_t)kq * 2 * (seq_t)r2 + (seq_t)(small / 4);
        unsigned nr = small % 4;
        q2 = q1; r2 = r1;
        q1 = nq; r1 = nr;
    }
    qB = q1; rB = r1;

    /* F(n) as 4*q + r */
    qF = 0; rF = 0;
    if (n % 2 == 0) {
        rF = 1;
        for (i = n / 2 + 1; i <= n; i++) a901_mulk(&qF, &rF, i);
    }

    rs = rF + rB;
    qs = qF + qB + (seq_t)(rs / 4);
    (void)rs;

    return T - qs;
}
