/*
 * A088020  a(n) = (n^2)!.
 * https://oeis.org/A088020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2)!
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* compute n^2 in seq_t and cast to idx_t for factorial */
    seq_t nn = (seq_t)n * (seq_t)n;
    idx_t m = (idx_t)nn;
    return factorial(m);
}
