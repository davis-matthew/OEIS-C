/*
 * A006259  Denominators of approximations to e.
 * https://oeis.org/A006259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) runs through the denominators of all intermediate fractions
 *            of the continued fraction of e = [2; 1,2,1, 1,4,1, 1,6,1, ...].
 *            For i = 0, 1, 2, ..., with partial quotients c(0)=2,
 *            c(i)=2(i+1)/3 if i == 2 (mod 3) and c(i)=1 otherwise, emit
 *            q(i-2) + k*q(i-1) for k = 1..c(i), where q(i) = c(i)*q(i...
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t qm2 = 1, qm1 = 0;   /* q(i-2), q(i-1) */
    idx_t rem = n;
    for (idx_t i = 0;; i++) {
        idx_t c;
        if (i == 0) c = 2;
        else if (i % 3 == 2) c = 2 * (i + 1) / 3;
        else c = 1;
        if (rem <= c) {
            seq_t r = qm2;
            r += (seq_t)rem * qm1;
            return r;
        }
        rem -= c;
        seq_t q = (seq_t)c * qm1;
        q += qm2;
        qm2 = qm1;
        qm1 = q;
    }
}
