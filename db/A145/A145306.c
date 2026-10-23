/*
 * A145306  Numbers n such that there exists x in N : (x+79)^3-x^3=n^2.
 * https://oeis.org/A145306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+2) = 456302*a(n+1) - a(n) with a(1) = 118579, a(2) =
 *            54107716279 (OEIS A145306)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* base cases */
    const seq_t A1 = (seq_t)118579;
    const seq_t A2 = (seq_t)54107716279ULL;

    if (n == 1) return A1;
    if (n == 2) return A2;

    /* iterative generation using two previous terms */
    seq_t prev2 = A1; /* a(n-2) */
    seq_t prev1 = A2; /* a(n-1) */
    seq_t cur   = 0;  /* a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        cur   = prev1 * (seq_t)456302 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
