/*
 * A101622  A Horadam-Jacobsthal sequence.
 * https://oeis.org/A101622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+2) + (-1)^n - 5)/2 Equivalent: if n even, a(n) =
 *            2^(n+1) - 2; if n odd, a(n) = 2^(n+1) - 3 (All arithmetic is
 *            performed in seq_t, the desired integer type.)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_mod.h"

seq_t a(idx_t n)
{
    seq_t p = pow2_mod(n + 1);          /* p = 2^(n+1)  (mod 2^width of seq_t) */
    return (n & 1ULL) ? (p - 3) : (p - 2);
}
