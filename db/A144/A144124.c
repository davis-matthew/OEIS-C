/*
 * A144124  P_4(2n+1), the Legendre polynomial of order 4 at 2n+1.
 * https://oeis.org/A144124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 70*n^4 + 140*n^3 + 90*n^2 + 20*n + 1
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* may use for type safety, not required */

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;           /* convert index to seq_t */
    seq_t n2   = nn * nn;            /* n^2 */
    seq_t n3   = n2 * nn;            /* n^3 */
    seq_t n4   = n3 * nn;            /* n^4 */

    seq_t ans = 70 * n4;             /* 70*n^4 */
    ans += 140 * n3;                 /* +140*n^3 */
    ans += 90 * n2;                  /* +90*n^2 */
    ans += 20 * nn;                  /* +20*n */
    ans += 1;                        /* +1 */

    return ans;
}
