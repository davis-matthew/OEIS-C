/*
 * A102081  Duplicate of A068397.
 * https://oeis.org/A102081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   A102081 (duplicate A068397) a(1) = 1, a(2) = 5, for n ≥ 3: if n
 *            is even: a(n) = a(n-1) + a(n-2) if n is odd: a(n) = a(n-1) +
 *            a(n-2) - 2
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                 /* safe guard, index 0 not used */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    seq_t prev2 = (seq_t)1; /* a(1) */
    seq_t prev1 = (seq_t)5; /* a(2) */
    seq_t cur = 0;
    for (idx_t i = 3; i <= n; ++i) {
        if ((i & 1) == 0) { /* even */
            cur = prev1 + prev2;
        } else {            /* odd */
            cur = prev1 + prev2 - (seq_t)2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
