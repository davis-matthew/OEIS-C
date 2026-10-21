/*
 * A211426  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and 2*w^2+x*y=0.
 * https://oeis.org/A211426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ordered triples (w,x,y) with all terms in {-n,…,0,…,n}
   satisfying 2*w^2 + x*y = 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* case w = 0 : x*y = 0 */
    result += (seq_t)(4 * n + 1);

    /* cases w > 0 and w < 0 handled together by doubling */
    for (idx_t absW = 1; absW <= n; ++absW) {
        seq_t w = (seq_t)absW;
        seq_t absP = (seq_t)2 * w * w;   /* |P| = 2*w^2 */

        seq_t add = 0;
        for (idx_t d = 1; d <= n; ++d) {
            seq_t dseq = (seq_t)d;
            /* stop when d^2 > |P| */
            if (dseq * dseq > absP) break;
            if (absP % dseq == 0) {
                seq_t eseq = absP / dseq;
                if (eseq <= (seq_t)n) {
                    if (dseq == eseq) {
                        add += 2;   /* (a,-a), (-a,a) */
                    } else {
                        add += 4;   /* (a,-b), (-a,b), (b,-a), (-b,a) */
                    }
                }
            }
        }
        /* each absW corresponds to w = ±absW */
        result += add * 2;
    }
    return result;
}
