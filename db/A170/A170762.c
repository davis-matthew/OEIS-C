/*
 * A170762  Expansion of g.f.: (1+x)/(1-42*x).
 * https://oeis.org/A170762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=43; a(n)=42*a(n-1) for n>=2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)43;
    /* for n >= 2 : a(n) = 43 * 42^(n-1) */
    seq_t p = pow_uint(42, n - 1);
    return (seq_t)43 * p;
}
