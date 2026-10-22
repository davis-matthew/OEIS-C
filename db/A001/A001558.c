/*
 * A001558  Number of hill-free Dyck paths of semilength n+3 and having length
 *          of first descent equal to 1 (a hill in a Dyck path is a peak at
 *          level 1).
 * https://oeis.org/A001558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000957(n+4) - A000957(n+3) - A000957(n+2), with Fine
 *            numbers from 2*A000957(k+1) + A000957(k) = C(k) (Catalan),
 *            A000957(1) = 1. Rewritten as a(n) = (C(n+3) - 3*A000957(n+3) -
 *            2*A000957(n+2))/2 so that no intermediate value is much larger
 *            than a(n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a001558_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001558_gcd.h"

seq_t a(idx_t n) {
    seq_t C = 1;   /* C(k) */
    seq_t G = 1;   /* A000957(k) */
    idx_t k;
    for (k = 1; k <= n + 1; k++) {
        seq_t Gn = (C - G) / 2;            /* A000957(k+1) */
        idx_t num = 2 * (2 * k + 1), den = k + 2;
        idx_t g = a001558_gcd(num, den);
        C = (C / (seq_t)(den / g)) * (seq_t)(num / g);   /* C(k+1) */
        G = Gn;
    }
    /* now C = C(n+2), G = A000957(n+2) */
    seq_t G2 = G;
    seq_t G3 = (C - G) / 2;                /* A000957(n+3) */
    idx_t num = 2 * (2 * n + 5), den = n + 4;
    idx_t g = a001558_gcd(num, den);
    idx_t t = num / g;
    seq_t u = C / (seq_t)(den / g);        /* C(n+3) = u*t */
    seq_t half = (u >> 1) * (seq_t)t + (u & 1) * (seq_t)(t >> 1);  /* floor(C(n+3)/2) */
    return half - G2 - G3 - (G3 >> 1);
}
