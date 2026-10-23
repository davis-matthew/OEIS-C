/*
 * A120727  a(n) = a(n-1) + a(n-2), starting with 110, 211.
 * https://oeis.org/A120727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 110, a(2) = 211
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the two defined starting values */
    if (n == 1) return 110;
    if (n == 2) return 211;

    /* Iterative computation of the linear recurrence. */
    seq_t prev2 = 110;   /* a(1) */
    seq_t prev1 = 211;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
