/*
 * A034474  a(n) = 5^n + 1.
 * https://oeis.org/A034474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

/* Computes a(n) = 5^n + 1 */
seq_t a(idx_t n)
{
    seq_t val = power5(n);
    return val + ((seq_t)1);
}
