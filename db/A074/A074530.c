/*
 * A074530  a(n) = 2^n + 3^n + 8^n.
 * https://oeis.org/A074530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^n + 8^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_seq_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_custom.h"

seq_t a(idx_t n)
{
    return pow_seq_custom((seq_t)2, n) +
           pow_seq_custom((seq_t)3, n) +
           pow_seq_custom((seq_t)8, n);
}
