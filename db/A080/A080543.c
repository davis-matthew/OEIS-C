/*
 * A080543  In binary representation: keep the first digit and rotate left the
 *          others twice.
 * https://oeis.org/A080543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = keep the first binary digit and rotate the remaining bits
 *            left by 2 positions
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   a080541
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a080541.h"

seq_t a(idx_t n)
{
    return a080541(n);
}
