/*
 * A195392  a(n) = (8*n)!.
 * https://oeis.org/A195392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    idx_t k = (idx_t)8 * n; /* compute 8 * n safely in idx_t */
    return factorial(k);   /* directly use the provided factorial routine */
}
