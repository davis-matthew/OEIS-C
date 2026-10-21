/*
 * A063439  a(n) = phi(n)^phi(n).
 * https://oeis.org/A063439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n)^phi(n)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t t = phi(n);
    return big_pow(t, t);
}
