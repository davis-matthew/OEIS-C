/*
 * A134368  a(n) = ((2n)!)^(n+1).
 * https://oeis.org/A134368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2n)!)^(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t two_n = n + n;
    seq_t f = factorial(two_n);
    return seq_pow(f, n + 1);
}
