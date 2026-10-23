/*
 * A146533  Catalan transform of A135092.
 * https://oeis.org/A146533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*C(2n,n) - 5*0^n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* 0^0 = 1 → (7*1-5)/2 = 1 */
        return 1;
    }

    /* central binomial coefficient C(2n,n) */
    seq_t c = binom(n + n, n);

    /* For n>0, C(2n,n) is even, so divide first to avoid overflow. */
    seq_t half = c / 2;

    return half * 7;
}
