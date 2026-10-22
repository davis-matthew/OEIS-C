/*
 * A006430  Number of loopless tree-rooted planar maps with 5 vertices and n
 *          faces and no isthmuses.
 * https://oeis.org/A006430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+2)*(n+3)*(23*n^10 + 963*n^9 + 17544*n^8 + 147952*n^7
 *            + 481675*n^6 - 1052153*n^5 - 7850914*n^4 - 2900162*n^3 +
 *            60869272*n^2 + 37067400*n - 179920800)/(2*11!) for n > 1, a(1) =
 *            0. Evaluated as a Newton series a(n) = Sum_{k=0..13} d_k *
 *            binomial(n-2, k), where d_k are the forward differe...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 93, u128 n <= 1000,
 *            big any n
 * Helpers:   a006430_binom, a006430_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006430_binom.h"
#include "../utils/a006430_small.h"

seq_t a(idx_t n) {
    long long d[14];
    int i, k;
    idx_t m;
    seq_t pos = 0, neg = 0;
    if (n < 2) return 0;
    for (i = 0; i < 14; i++) d[i] = a006430_small((long long)i + 2);
    for (k = 1; k < 14; k++)
        for (i = 13; i >= k; i--)
            d[i] -= d[i - 1];
    m = n - 2;
    for (k = 0; k < 14; k++) {
        if (d[k] == 0) continue;
        if (d[k] > 0)
            pos += (seq_t)(unsigned long long)d[k] * a006430_binom(m, k);
        else
            neg += (seq_t)(unsigned long long)(-d[k]) * a006430_binom(m, k);
    }
    return pos - neg;
}
