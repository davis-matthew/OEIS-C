/*
 * A002721  Number of 3 X 3 X 3 arrays M_ijk (1 <= i,j,k <= 3) with entries
 *          satisfying 0 <= M_ijk <= n and all line sums equal to n.
 * https://oeis.org/A002721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..8} c_k * binomial(n+8-k, 8), with c =
 *            [1,3,60,7,168,7,60,3,1] (from G.f.
 *            (1+3x+60x^2+7x^3+168x^4+7x^5+60x^6+3x^7+x^8)/(1-x)^9); equals
 *            (1/4032)*m*(m*(m*(31*m+1004)+6820)+4272)+1 with m = n*(n+1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 28, u64 n <= 469, u128 n <= 1000,
 *            big any n
 * Helpers:   a2721_binom8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a2721_binom8.h"

seq_t a(idx_t n) {
    static const unsigned c[9] = {1, 3, 60, 7, 168, 7, 60, 3, 1};
    seq_t s = 0;
    int k;
    for (k = 0; k <= 8; k++) {
        idx_t m = n + 8 - (idx_t)k;
        seq_t b = a2721_binom8(m);
        s += b * c[k];
    }
    return s;
}
