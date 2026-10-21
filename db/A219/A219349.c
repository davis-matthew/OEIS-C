/*
 * A219349  Number of n X 3 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random, but sorted with lexicographically
 *          nondecreasing rows and columns, 0..1 n X 3 array.
 * https://oeis.org/A219349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 6·n^3 + 23·n^2 + 78·n - 192) / 24 (n > 3); *
 *            Initial values: a(1) = 3, a(2) = 3, a(3) = 8
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first three terms directly */
    if (n == 1 || n == 2) return (seq_t)3;
    if (n == 3)          return (seq_t)8;

    /* All calculations are performed in seq_t, taking care to avoid
     * underflows when subtracting by using two's‑complement arithmetic. */

    const seq_t N = (seq_t)n;
    const seq_t N2 = N * N;                    /* n^2   */
    const seq_t N3 = N2 * N;                   /* n^3   */
    const seq_t N4 = N3 * N;                   /* n^4   */

    const seq_t t2 = (seq_t)6  * N3;           /* 6 n^3 */
    const seq_t t3 = (seq_t)23 * N2;           /* 23 n^2*/
    const seq_t t4 = (seq_t)78 * N;            /* 78 n  */
    const seq_t t5 = (seq_t)192;               /* 192   */

    seq_t num = N4;                            /* n^4 */
    num += t3;                                 /* + 23 n^2 */
    num += t4;                                 /* + 78 n  */
    num += (~t2 + (seq_t)1);                   /* - 6 n^3 */
    num += (~t5 + (seq_t)1);                   /* - 192   */

    return num / (seq_t)24;                    /* divide by 24 */
}
