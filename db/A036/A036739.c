/*
 * A036739  a(n) = (n!)^n+1.
 * https://oeis.org/A036739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^n + 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   factorial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    seq_t p = seq_pow(f, n);
    return p + 1;
}
