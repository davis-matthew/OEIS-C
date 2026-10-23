/*
 * A132652  Sum of divisors of n, sigma(n) raised to power sigma(n).
 * https://oeis.org/A132652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n)^sigma(n) (sigma is the sum-of-divisors function)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    return big_pow(sigma, sigma);
}
