/*
 * A021666  Decimal expansion of 1/662.
 * https://oeis.org/A021666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 662) / 662)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binary exponentiation of 10 modulo 662 */
static seq_t modpow10(idx_t exp)
{
    uint64_t result = 1;
    uint64_t base   = 10;
    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % 662ULL;
        base = (base * base) % 662ULL;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    seq_t rem   = modpow10(n);          /* r = (10^n) mod 662        */
    seq_t digit = (rem * 10) / 662;     /* floor(10*r/662)            */
    return digit;
}
