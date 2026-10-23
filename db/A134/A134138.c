/*
 * A134138  Alternating row sums of triangle A046089 (S1p(3)).
 * https://oeis.org/A134138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (3*n-4)*a(n-1) - 3*(n-2)*(n-1)*a(n-2) +
 *            (n-3)*(n-2)*(n-1)*a(n-3) with initial values a(1)=1, a(2)=2,
 *            a(3)=4
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    seq_t a0 = (seq_t)1; /* a(1)   */
    seq_t a1 = (seq_t)2; /* a(2)   */
    seq_t a2 = (seq_t)4; /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = (seq_t)(3 * i - 4) * a2;
        seq_t t2 = -((seq_t)(3 * (i - 2) * (i - 1)) * a1);
        seq_t t3 = (seq_t)((i - 3) * (i - 2) * (i - 1)) * a0;
        seq_t next = t1 + t2 + t3;

        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
