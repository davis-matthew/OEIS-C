/*
 * A224354  Number of 3 X n 0..2 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A224354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (23*n^6 + 171*n^5 + 1175*n^4 + 2745*n^3 + 3842*n^2 +
 *            6804*n + 1080)/360 (for n>1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 22, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)27;

    /* convert index to seq_t */
    seq_t sn = (seq_t)n;

    /* compute successive powers of n */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t n5 = n4 * sn;          /* n^5 */
    seq_t n6 = n5 * sn;          /* n^6 */

    /* build numerator of the rational expression */
    seq_t num = (seq_t)23 * n6;
    num += (seq_t)171 * n5;
    num += (seq_t)1175 * n4;
    num += (seq_t)2745 * n3;
    num += (seq_t)3842 * n2;
    num += (seq_t)6804 * sn;
    num += (seq_t)1080;

    /* divide by 360 */
    return num / (seq_t)360;
}
