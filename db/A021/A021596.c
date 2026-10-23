/*
 * A021596  Decimal expansion of 1/592.
 * https://oeis.org/A021596
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

/* 
 * Sequence A021596: Decimal expansion of 1/592.
 * a(n) = nth digit after the decimal point in the decimal
 *        representation of 1/592.
 * This is computed as:
 *   a(n) = floor( 10 * (10^n mod 592) / 592 )
 * where n >= 0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus for the arithmetic */
    const uint16_t mod = 592;

    /* Compute r = 10^n mod 592 using binary exponentiation. */
    uint16_t r = 1;                 /* r = 10^0 mod 592 */
    uint16_t base = 10 % mod;       /* base = 10^1 mod 592 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            r = (uint16_t)((r * base) % mod);
        }
        base = (uint16_t)((base * base) % mod);
        exp >>= 1;
    }

    /* The nth digit is floor(10 * r / 592).  r < 592, so
     * the product fits comfortably in a 16‑bit integer. */
    uint16_t digit = (uint16_t)((r * 10) / mod);

    return (seq_t)digit;
}
