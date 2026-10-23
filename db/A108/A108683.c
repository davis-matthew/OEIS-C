/*
 * A108683  a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(7*n^2 + 20*n + 15)/360.
 * https://oeis.org/A108683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(7*n^2 + 20*n + 15)/360
 * Range:     u8 none, u16 n <= 10, u32 n <= 75, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_seq_local.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t num[5];
    num[0] = sn + 1;
    num[1] = sn + 2;
    num[2] = sn + 3;
    num[3] = sn + 4;
    num[4] = 7 * sn * sn + 20 * sn + 15;   /* 7*n^2 + 20*n + 15 */

    seq_t denom = (seq_t)360;               /* denominator */

    /* Cancel common factors with the denominator */
    while (denom > 1) {
        int changed = 0;
        for (int i = 0; i < 5; ++i) {
            seq_t g = gcd_seq_local(num[i], denom);
            if (g > 1) {
                num[i] /= g;
                denom /= g;
                changed = 1;
                if (denom == 1) break;
            }
        }
        if (!changed) break;  /* should not happen for valid formula */
    }

    /* Multiply the remaining factors */
    seq_t res = 1;
    for (int i = 0; i < 5; ++i) {
        res *= num[i];
    }
    return res;
}
