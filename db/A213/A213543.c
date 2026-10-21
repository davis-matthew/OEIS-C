/*
 * A213543  a(n) = n AND 3^n, where AND is the bitwise AND operator.
 * https://oeis.org/A213543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look, less
 * Formula:   a(n) = n AND 3^n
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);          /* 3^n, computed in seq_t */
    return ((seq_t)n) & p;      /* bitwise AND of n with 3^n */
}
