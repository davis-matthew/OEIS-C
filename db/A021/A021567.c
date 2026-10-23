/*
 * A021567  Decimal expansion of 1/563.
 * https://oeis.org/A021567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal expansion of 1/563
   a(n) = ⌊10 * (10^n mod 563) / 563⌋
   a is the (n+1)-th decimal digit of 1/563 after the decimal point
   (the first digit is a(0), the second a(1), …). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 10^n (mod 563) by binary exponentiation */
    uint64_t exp   = n;
    uint64_t base  = 10 % 563;
    uint64_t mod   = 563;
    uint64_t result = 1;

    while (exp > 0) {
        if (exp & 1U) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1U;
    }

    /* digit = floor((10 * result) / 563) */
    uint64_t digit = (10 * result) / 563;
    return (seq_t)digit;
}
