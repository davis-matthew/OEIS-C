/*
 * A078704  Integer part of the square root of phi(n).
 * https://oeis.org/A078704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = floor(sqrt(phi(n)))
 * Range:     u8 n <= 46, u16 n <= 5314, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    seq_t t = phi(n);
    return sqrt_floor(t);
}
