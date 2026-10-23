/*
 * A176128  a(n) = (n*n!)^2.
 * https://oeis.org/A176128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n*n!)^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);
    seq_t prod = fact * (seq_t)n;
    return prod * prod;
}
