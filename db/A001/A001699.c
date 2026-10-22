/*
 * A001699  Number of binary trees of height n; or products (ways to insert
 *          parentheses) of height n when multiplication is non-commutative
 *          and non-associative.
 * https://oeis.org/A001699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = A003095(n+1) - A003095(n), where A003095(0) = 0,
 *            A003095(k+1) = A003095(k)^2 + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t b = 0;      /* A003095(k) */
    seq_t next;
    idx_t k;
    for (k = 0; k < n; k++) {
        b = b * b + 1;
    }
    next = b * b + 1; /* A003095(n+1) */
    return next - b;
}
