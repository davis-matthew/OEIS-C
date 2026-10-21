/*
 * A055426  Number of points in Z^n of norm <= 2.
 * https://oeis.org/A055426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 2*n + 16*n^2 - 8*n^3 + 2*n^4) / 3
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 243, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2   */
    seq_t n3 = n2 * nn;          /* n^3   */
    seq_t n4 = n3 * nn;          /* n^4   */
    seq_t term = 3 + 2 * nn + 16 * n2 + 2 * n4;
    seq_t sub = 8 * n3;
    seq_t numerator = term - sub;   /* term >= sub for all n >= 0 */
    return numerator / 3;
}
