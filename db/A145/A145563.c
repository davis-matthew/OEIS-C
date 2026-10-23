/*
 * A145563  a(0)=0 and a(n+1) = 3*a(n) + 2^(n+2).
 * https://oeis.org/A145563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4*(3^n - 2^n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* 3^n - 2^n is always non‑negative, so the result fits in seq_t. */
    seq_t p3 = pow3(n);
    seq_t p2 = pow2(n);
    return (seq_t)4 * (p3 - p2);
}
