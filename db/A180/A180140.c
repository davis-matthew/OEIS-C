/*
 * A180140  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.:
 *          (1+x+x^2)/(1-3*x-5*x^2).
 * https://oeis.org/A180140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 5*a(n-2) for n ≥ 3 a(0) = 1, a(1) = 4, a(2) =
 *            18
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)18;

    /* iterative computation of the linear recurrence */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t prev1 = (seq_t)18;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)3 * prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
