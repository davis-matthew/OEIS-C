/*
 * A005373  A class of rooted trees with n nodes.
 * https://oeis.org/A005373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. T(x) satisfies T = x + x*T + x*T^2/2 + T^2/2, i.e. T = (1
 *            - x - sqrt(1-4x-x^2))/(1+x). With A(n) = a(n) + n*a(n-1): A(n+1)
 *            = (4n-2)*A(n) + n*(n-2)*A(n-1) for n >= 2, A(1) = 1, A(2) = 5,
 *            and a(n) = A(n) - n*a(n-1), a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 3;
    seq_t Aprev = 1;   /* A(1) */
    seq_t Acur = 5;    /* A(2) */
    seq_t an = 3;      /* a(2) */
    for (idx_t k = 2; k < n; k++) {
        /* compute A(k+1) */
        seq_t Anext = ((seq_t)4 * (seq_t)k - (seq_t)2) * Acur
                    + (seq_t)k * (seq_t)(k - 2) * Aprev;
        an = Anext - (seq_t)(k + 1) * an;
        Aprev = Acur;
        Acur = Anext;
    }
    return an;
}
