/*
 * A032548  Integer part of decimal 'base-7 looking' numbers divided by their
 *          actual base-7 values.
 * https://oeis.org/A032548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(A007093(n)/n) where A007093(n) is the decimal
 *            number obtained by interpreting the base-7 representation of n
 *            as decimal digits
 * Range:     u8 n <= 866, u16 n <= 9639, u32 n <= 9639, u64 n <= 9639,
 *            u128 n <= 9639, big any n
 * Helpers:   dec_from_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/dec_from_base7.h"

seq_t a(idx_t n)
{
    seq_t m = dec_from_base7(n);
    return m / (seq_t)n;
}
