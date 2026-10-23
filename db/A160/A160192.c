/*
 * A160192  Numerator of Hermite(n, 3/28).
 * https://oeis.org/A160192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n)=3*a(n-1)-392*(n-1)*a(n-2) (with a(0)=1, a(1)=3)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 3;

    seq_t prev = 1;  /* a(0) */
    seq_t curr = 3;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = 392 * (seq_t)(i - 1);
        seq_t next = 3 * curr - mult * prev;
        prev = curr;
        curr = next;
    }

    return curr;
}
