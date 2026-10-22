/*
 * A002468  The game of Mousetrap with n cards: the number of permutations of
 *          n cards having at least one hit after 2.
 * https://oeis.org/A002468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)! - (n-2)! - (E(n-4,3) - 4*E(n-4,2) + 3*E(n-4,1))
 *            for n >= 4, where E(m,j) = (m+j-1)*E(m-1,j) + (m-1)*E(m-2,j),
 *            E(0,j) = j!, E(1,j) = j*j! (E(m,j) = permutations of m+j
 *            elements with m given elements not fixed); a(1) = a(2) = 0, a(3)
 *            = 1. Equivalent to a(n) = A001563(n) - A002469(n+2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   a002468_pd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002468_pd.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 1;
    idx_t M = n - 4;
    seq_t e3 = a002468_pd(M, 3);
    seq_t e2 = a002468_pd(M, 2);
    seq_t e1 = a002468_pd(M, 1);
    seq_t f2 = 1;
    for (idx_t i = 2; i <= n - 2; i++) f2 = f2 * (seq_t)i;
    seq_t f1 = f2 * (seq_t)(n - 1);
    seq_t pos = f1 + e2 * 4;
    seq_t neg = f2 + e3 + e1 * 3;
    return pos - neg;
}
