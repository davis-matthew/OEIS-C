/*
 * A036740  a(n) = (n!)^n.
 * https://oeis.org/A036740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^n
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    return big_pow(fact, (seq_t)n);
}
