/*
 * A194590  a(n) = (-1)^n*(A056040(n+1)*A152271(n)-2^n)/2.
 * https://oeis.org/A194590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * (A056040(n+1) * A152271(n) - 2^n) / 2 Computed
 *            via simple recurrences that avoid the need for the auxiliary
 *            sequences A056040 and A152271. Let b_k = C(2k-1, k-1). Then
 *            a(2k) = 4 * a(2k-2) + b_k a(2k+1) = 4 * a(2k-1) - 2 * b_k where
 *            a(0) = a(1) = 0. The binomial C(2k-1, k-1) is eva...
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 30, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t, if needed */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;

    seq_t a_even = 0;   /* a(0)  */
    seq_t a_odd  = 0;   /* a(1)  */

    for (idx_t k = 1; ; ++k) {
        /* compute b_k = C(2k-1, k-1)  multiplicatively   */
        seq_t b = 1;
        for (idx_t i = 1; i <= k - 1; ++i) {
            b = b * (seq_t)(k + i) / (seq_t)i;
        }

        /* even term  a(2k) */
        seq_t even_val = (seq_t)4 * a_even + b;
        if ((idx_t)(2 * k) == n) return even_val;
        a_even = even_val;

        /* odd term   a(2k+1) */
        seq_t odd_val = (seq_t)4 * a_odd - (seq_t)2 * b;
        if ((idx_t)(2 * k + 1) == n) return odd_val;
        a_odd = odd_val;
    }
}
