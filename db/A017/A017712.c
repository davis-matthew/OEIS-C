/*
 * A017712  Denominator of sum of -24th powers of divisors of n.
 * https://oeis.org/A017712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^24 / gcd( sigma_24(n), n^24 )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, gcd, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/gcd.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t n_ts = (seq_t)n;
    seq_t n24  = big_pow(n_ts, (seq_t)24);
    seq_t sigma = sigma_k(n, (uint8_t)24);
    seq_t g = gcd(n24, sigma);
    return n24 / g;
}
