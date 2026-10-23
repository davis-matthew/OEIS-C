/*
 * A158780  a(2n) = A131577(n), a(2n+1) = A011782(n).
 * https://oeis.org/A158780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^floor(n/2) + [n=1] - [n=0]) / 2
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the integer in the OEIS sequence A158780 for any n ≥ 0. */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Special cases for the first two terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* For n ≥ 2, a(n) = 2^(floor(n/2)-1). */
    idx_t exp = (n >> 1) - 1;      /* floor(n/2) - 1 */
    return pow2(exp);              /* pow2 returns a seq_t value */
}
