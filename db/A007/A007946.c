/*
 * A007946  a(n) = 6*(2*n+1)! / ((n!)^2*(n+3)).
 * https://oeis.org/A007946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2n+2, n) + C(2n+3, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   sbinom, sgcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sbinom.h"
#include "../utils/sgcd.h"

seq_t a(idx_t n)
{
    return sbinom(2 * n + 2, n) + sbinom(2 * n + 3, n);
}
