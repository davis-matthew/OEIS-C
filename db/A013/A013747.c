/*
 * A013747  a(n) = 10^(3*n + 2).
 * https://oeis.org/A013747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^(3*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Computes the OEIS sequence A013747:
 *   a(n) = 10^(3*n + 2)
 *   with offset 0, so a(0) = 100. */
seq_t a(idx_t n)
{
    idx_t exp = n * 3 + 2;
    return pow10(exp);
}
