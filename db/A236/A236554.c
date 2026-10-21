/*
 * A236554  Number of involutions in the quaternion ring over Z/(2^n)Z with
 *          i^2=j^2=1.
 * https://oeis.org/A236554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(1)=8, a(2)=64; for n ≥ 3: a(n) = 4^(n+1) + 32
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return 8;
    if (n == 2) return 64;
    return pow2(2 * (n + 1)) + (seq_t)32;
}
