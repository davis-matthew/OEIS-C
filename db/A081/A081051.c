/*
 * A081051  Stirling numbers of the first kind.
 * https://oeis.org/A081051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = coefficient of x^3 in Π_{k=0}^{n-1} (x - k). This is the
 *            signed Stirling number of the first kind s(n+1,3). Recurrence:
 *            s(m,3) = s(m-1,2) - (m-1)*s(m-1,3) with s(1,3)=0, s(2,3)=0,
 *            s(3,3)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* We iterate up to m = n+1 because a(n) = s(n+1,3) */
    seq_t s0 = 1;   /* s(i,0) */
    seq_t s1 = 0;   /* s(i,1) */
    seq_t s2 = 0;   /* s(i,2) */
    seq_t s3 = 0;   /* s(i,3) */

    for (idx_t i = 1; i <= n + 1; ++i) {
        seq_t new0 = 0; /* s(i,0) = 0 for i>0 */
        seq_t new1 = s0 - (seq_t)(i - 1) * s1;
        seq_t new2 = s1 - (seq_t)(i - 1) * s2;
        seq_t new3 = s2 - (seq_t)(i - 1) * s3;
        s0 = new0;
        s1 = new1;
        s2 = new2;
        s3 = new3;
    }
    return s3;
}
