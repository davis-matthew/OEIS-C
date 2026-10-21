/*
 * A089800  Expansion of Jacobi theta function theta_2(q)/q^(1/4).
 * https://oeis.org/A089800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is a pronic number (n = k*(k+1)), 0 otherwise.
 *            Equivalent to a(n) =
 *            2*(floor(sqrt(n+1)+1/2)-floor(sqrt(n)+1/2)). We detect pronic
 *            numbers by computing k = floor(sqrt(n)) and checking whether
 *            k*(k+1) == n, using 128‑bit arithmetic to avoid overflow
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_uint64.h"

seq_t a(idx_t n)
{
    idx_t k = floor_sqrt_uint64(n);
    unsigned __int128 prod = (unsigned __int128)k * (k + 1);
    return (seq_t)(prod == n ? 2 : 0);
}
