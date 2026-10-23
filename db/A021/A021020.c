/*
 * A021020  Decimal expansion of 1/16.
 * https://oeis.org/A021020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/16) mod 10 = (10^(n+1) mod 160) / 16 The
 *            decimal digits of 1/16 are 0,6,2,5,0,0,
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 10^(n+1) mod 160 via exponentiation by squaring. */
    uint64_t exp   = n + 1;            /* exponent (n+1) */
    uint64_t r     = 1 % 160;          /* current result modulo 160 */
    uint64_t base  = 10 % 160;         /* base modulo 160 */

    while (exp)
    {
        if (exp & 1ULL)
            r = (r * base) % 160;
        base = (base * base) % 160;
        exp >>= 1ULL;
    }

    return (seq_t)(r / 16);            /* integer division gives the digit */
}
