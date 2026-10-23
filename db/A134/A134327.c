/*
 * A134327  a(n) = (n^5-n-5)/5.
 * https://oeis.org/A134327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n^5 - n - 5)/5
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t n_t = (seq_t)n;
    seq_t n2 = n_t * n_t;          /* n^2 */
    seq_t n3 = n2 * n_t;           /* n^3 */
    seq_t n4 = n3 * n_t;           /* n^4 */
    seq_t n5 = n4 * n_t;           /* n^5 */
    return (n5 - n_t - 5) / 5;     /* (n^5 - n - 5) / 5 */
}
