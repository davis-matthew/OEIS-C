/*
 * A021953  Decimal expansion of 1/949.
 * https://oeis.org/A021953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * (10^n mod 949) / 949)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: modular exponentiation of 10 modulo 949 */
static seq_t modpow10(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 10 % 949;
    seq_t mod    = 949;

    while (exp != 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t rem = modpow10(n);           /* rem = 10^n mod 949 */
    /* compute digit = floor(10 * rem / 949) safely using native integer */
    uint16_t digit = ((uint16_t)10 * (uint16_t)rem) / 949;
    return (seq_t)digit;
}
