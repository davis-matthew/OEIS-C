/*
 * A220004  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical or antidiagonal neighbors
 *          in a random, but sorted with lexicographically nondecreasing rows
 *          and nonincreasing columns, 0..3 n X 2 array.
 * https://oeis.org/A220004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=7, a(3)=20 * For n>3 * a(n) = ( n^8 - 4*n^7 -
 *            14*n^6 + 2408*n^5 - 20111*n^4 * + 109004*n^3 - 40356*n^2 -
 *            494448*n + 725760 ) / 40320 * (All divisions are exact. The
 *            numerator is guaranteed to be a multiple * of 40320.)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)20;

    /* Compute powers of n */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;
    seq_t n8 = n7 * n1;

    /* Compute numerator = positive terms - negative terms */
    seq_t positive = n8;
    positive += (seq_t)2408 * n5;        /* +2408*n^5 */
    positive += (seq_t)109004 * n3;      /* +109004*n^3 */
    positive += (seq_t)725760;           /* +725760   */

    seq_t negative = (seq_t)4 * n7;      /* -4*n^7     */
    negative += (seq_t)14 * n6;          /* -14*n^6    */
    negative += (seq_t)20111 * n4;       /* -20111*n^4 */
    negative += (seq_t)40356 * n2;       /* -40356*n^2 */
    negative += (seq_t)494448 * n1;      /* -494448*n  */

    /* Numerator is positive - negative; guaranteed non‑negative  */
    seq_t numerator = positive - negative;

    /* Exact division by 40320 */
    return numerator / (seq_t)40320;
}
