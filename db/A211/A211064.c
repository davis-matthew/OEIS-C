/*
 * A211064  Number of 2 X 2 matrices having all terms in {1,...,n} and even
 *          determinant.
 * https://oeis.org/A211064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - (2*n + 1 -(-1)^n)^2*(6*n + 1 -(-1)^n)*(2*n - 1 +
 *            (-1)^n)/128
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 286, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not used (offset is 1), but safe */
    seq_t n_s = (seq_t)n;
    /* compute (-1)^n as +1 for even n, -1 for odd n */
    int64_t s = (n & 1) ? -1 : 1;

    /* evaluate the parts (they are non‑negative) */
    int64_t part1 = (int64_t)2 * (int64_t)n + 1 - s;          /* 2*n + 1 - (-1)^n */
    int64_t part2 = (int64_t)6 * (int64_t)n + 1 - s;          /* 6*n + 1 - (-1)^n */
    int64_t part3 = (int64_t)2 * (int64_t)n - 1 + s;          /* 2*n - 1 + (-1)^n */

    /* cast to seq_t for multiplication */
    seq_t p1 = (seq_t)part1;
    seq_t p2 = (seq_t)part2;
    seq_t p3 = (seq_t)part3;

    /* numerator of the correction term: (p1^2) * p2 * p3 */
    seq_t numer = p1 * p1;
    numer = numer * p2;
    numer = numer * p3;

    /* compute n^4 */
    seq_t n2 = n_s * n_s;               /* n^2 */
    seq_t n4 = n2 * n2;                 /* n^4 */

    /* subtract the correction divided by 128 */
    seq_t sub = numer / 128;            /* exact division */
    return n4 - sub;
}
