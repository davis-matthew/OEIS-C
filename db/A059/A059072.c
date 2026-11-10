/*
 * A059072  Duplicate of A000459.
 * https://oeis.org/A059072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n*(2*n-1)*a(n-1) + 2*n*(n-1)*a(n-2) - (2*n-1), a(0) = 1,
 *            a(1) = 0
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A000459_a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;

    seq_t a_prev2 = 1;
    seq_t a_prev1 = 0;

    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t a_curr = i_seq * (2 * i_seq - 1) * a_prev1
                     + 2 * i_seq * (i_seq - 1) * a_prev2
                     - (2 * i_seq - 1);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}

seq_t a(idx_t n) {
    return A000459_a(n);
}
