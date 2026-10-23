/*
 * A021462  Decimal expansion of 1/458.
 * https://oeis.org/A021462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 458) mod 10 (decimal digit at position n
 *            of 1/458)
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* 458 is the divisor, 4580 = 458 * 10 */
    const seq_t MOD  = 458;
    const seq_t MOD1 = 4580;

    /* We need 10^(n+1) to get the digit for index n */
    idx_t exp = n + 1;

    /* 10^(n+1) mod 458  and  mod 4580 */
    seq_t r  = mod_pow((seq_t)10, exp, MOD);   /* remainder modulo 458 */
    seq_t s  = mod_pow((seq_t)10, exp, MOD1);  /* remainder modulo 4580 */

    /* (s - r) is a multiple of 458.  Use (s + MOD1 - r) to avoid underflow. */
    seq_t diff = s + MOD1 - r;
    seq_t q    = diff / MOD;   /* q = floor(10^(n+1)/458) modulo 10 */

    return q % 10;
}
