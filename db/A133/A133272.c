/*
 * A133272  Indices of centered heptagonal numbers (A069099) which are also
 *          heptagonal numbers (A000566).
 * https://oeis.org/A133272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 7, a(n+2) = 12*a(n+1) – a(n) – 5 (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)7;
    }
    seq_t a_prevprev = (seq_t)1; /* a(1) */
    seq_t a_prev     = (seq_t)7; /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        curr = 12 * a_prev - a_prevprev - (seq_t)5;
        a_prevprev = a_prev;
        a_prev = curr;
    }
    return a_prev;
}
