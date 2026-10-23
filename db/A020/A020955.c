/*
 * A020955  a(n) = n^(2^n - n - 1).
 * https://oeis.org/A020955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n^(2^n - n - 1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t base = (seq_t)n;
    seq_t exp  = pow2(n) - (seq_t)n - (seq_t)1;
    return big_pow(base, exp);
}
