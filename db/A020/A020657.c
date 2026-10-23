/*
 * A020657  Lexicographically earliest increasing sequence of nonnegative
 *          numbers that contains no arithmetic progression of length 7.
 * https://oeis.org/A020657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = interpret (n-1) in base‑6 as a base‑7 number. This is the
 *            lexicographically earliest increasing sequence of nonnegative
 *            integers that contains no arithmetic progression of length 7
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base6_to_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/base6_to_base7.h"

seq_t a(idx_t n)
{
    return base6_to_base7(n - 1);
}
