/*
 * A221545  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^6 + 3*n^5 - 8*n^4 + 25*n^3 - 30*n^2 + 20*n - 9 for n >
 *            3 a(1) = 8, a(2) = 134, a(3) = 1276
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* handle the offset and small values */
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)134;
    if (n == 3) return (seq_t)1276;

    /* use the empirical polynomial for all n > 3 */
    seq_t N  = (seq_t)n;          /* convert index once */
    seq_t N2 = N * N;             /* n^2 */
    seq_t N3 = N2 * N;            /* n^3 */
    seq_t N4 = N3 * N;            /* n^4 */
    seq_t N5 = N4 * N;            /* n^5 */
    seq_t N6 = N5 * N;            /* n^6 */

    /* compute the polynomial */
    seq_t val = N6;                 /* n^6 */
    val += (seq_t)3 * N5;           /* +3*n^5 */
    val -= (seq_t)8 * N4;           /* -8*n^4 */
    val += (seq_t)25 * N3;          /* +25*n^3 */
    val -= (seq_t)30 * N2;          /* -30*n^2 */
    val += (seq_t)20 * N;           /* +20*n */
    val -= (seq_t)9;                /* -9 */

    return val;
}
