/*
 * A084635  Binomial transform of 1,0,1,0,1,1,1,...
 * https://oeis.org/A084635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - n*(n^2 - 3*n + 8)/6
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= 2;

    /* compute sub = n*(n^2 - 3*n + 8)/6 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                     /* n^2   */
    seq_t expr = n2 - 3*nn + 8;             /* n^2-3n+8 */
    seq_t sub = (nn * expr) / 6;

    return pow2 - sub;
}
