/*
 * A211033  Number of 2 X 2 matrices having all elements in {0,1,...,n} and
 *          determinant = 0 (mod 3).
 * https://oeis.org/A211033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n mod 3 == 0: (11*n^4 + 60*n^3 + 138*n^2 + 108*n)/27 +
 *            1 if n mod 3 == 1: (11*n^4 + 52*n^3 + 96*n^2 + 76*n + 35)/27 if
 *            n mod 3 == 2: 11*(n + 1)^4/27 (All divisions are exact.)
 * Range:     u8 n <= 0, u16 n <= 7, u32 n <= 239, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = n;       /* promote index to seq_t for all arithmetic */
    idx_t r = n % 3;

    if (r == 0) {
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;
        seq_t n4 = n3 * nn;
        seq_t num = 11 * n4 + 60 * n3 + 138 * n2 + 108 * nn;
        return num / (seq_t)27 + 1;
    } else if (r == 1) {
        seq_t n2 = nn * nn;
        seq_t n3 = n2 * nn;
        seq_t n4 = n3 * nn;
        seq_t num = 11 * n4 + 52 * n3 + 96 * n2 + 76 * nn + 35;
        return num / (seq_t)27;
    } else { /* r == 2 */
        seq_t m = nn + 1;
        seq_t m2 = m * m;
        seq_t m3 = m2 * m;
        seq_t m4 = m3 * m;
        seq_t num = 11 * m4;
        return num / (seq_t)27;
    }
}
