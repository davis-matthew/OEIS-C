/*
 * A100185  Structured meta-anti-prism numbers, the n-th number from a
 *          structured n-gonal anti-prism number sequence.
 * https://oeis.org/A100185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(3*n^4 - 8*n^3 + 9*n^2 + 2*n)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 304, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* OEIS offset is 1: a(1) = 1.  For n==0 we can return 0. */
    if (n == 0) return 0;
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;        /* n^2 */
    seq_t n3 = n2 * s;       /* n^3 */
    seq_t n4 = n3 * s;       /* n^4 */
    seq_t num = 3 * n4 - 8 * n3 + 9 * n2 + 2 * s;
    return num / 6;
}
