/*
 * A156269  Denominators of a series expansion for Pi/2.
 * https://oeis.org/A156269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(3*n) = (-4)^n * (4*n + 1) a(3*n+1) = (-4)^n * (4*n + 2)
 *            a(3*n+2) = (-4)^n * (8*n + 6)
 * Range:     i8 n <= 5, i16 n <= 16, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = n / 3;
    int r = n % 3;

    /* compute (-4)^k as a seq_t */
    seq_t pow = 1;
    for (idx_t i = 0; i < k; ++i)
        pow *= -4;

    /* linear factor depending on n mod 3 */
    seq_t factor;
    switch (r) {
        case 0:  factor = (seq_t)(4 * k + 1); break;
        case 1:  factor = (seq_t)(4 * k + 2); break;
        case 2:  factor = (seq_t)(8 * k + 6); break;
        default: factor = 0;  /* unreachable */
    }

    return pow * factor;
}
