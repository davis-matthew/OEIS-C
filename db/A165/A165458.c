/*
 * A165458  a(0)=1, a(1)=4, a(n) = 12*a(n-2) - a(n-1).
 * https://oeis.org/A165458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 12*a(n-2) - a(n-1); a(0) = 1, a(1) = 4
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a_nm2 = (seq_t)1; /* a(n-2) for n = 2 starts as a(0) */
    seq_t a_nm1 = (seq_t)4; /* a(n-1) for n = 2 starts as a(1) */
    seq_t an = a_nm2;       /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)12 * a_nm2 - a_nm1;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
