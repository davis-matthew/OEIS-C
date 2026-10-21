/*
 * A087968  a(n) = gcd(1 + 2^n, n^2).
 * https://oeis.org/A087968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(1 + 2^n, n^2)
 * Range:     u8 n <= 15, u16 n <= 1538, u32 n <= 9535, u64 n <= 9535,
 *            u128 n <= 9535, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n^2 as the modulus (wrapped into seq_t space) */
    seq_t mod = (seq_t)n * (seq_t)n;
    /* compute 2^n modulo n^2 */
    seq_t pow2 = mod_pow((seq_t)2, n, mod);
    /* 1 + 2^n modulo n^2 */
    seq_t val = pow2 + (seq_t)1;
    /* gcd of val and n^2 */
    return gcd(val, mod);
}
