/*
 * A199758  a(n) = 7*11^n + 1.
 * https://oeis.org/A199758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*11^n + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 8, a(1) = 78, a(2) = 848, ... */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t p = seq_pow((seq_t)11, n);
    return p * 7 + 1;
}
