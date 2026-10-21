/*
 * A228341  Third diagonal (T(n,2)) of triangle in A228340.
 * https://oeis.org/A228341
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * a(n-1) + a(n-2) with a(2) = 1, a(3) = 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* Handle the two known initial values */
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Iterative computation using the recurrence.
       No memoisation or dynamic memory is required.  */
    seq_t a_n_minus_two = (seq_t)1; /* a(2) */
    seq_t a_n_minus_one = (seq_t)2; /* a(3) */
    seq_t a_n = 0;

    for (idx_t k = 4; k <= n; ++k) {
        a_n = (seq_t)k * a_n_minus_one + a_n_minus_two;
        a_n_minus_two = a_n_minus_one;
        a_n_minus_one = a_n;
    }

    return a_n;
}
