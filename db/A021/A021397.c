/*
 * A021397  Decimal expansion of 1/393.
 * https://oeis.org/A021397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod 393)) / 393)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 393 using fast exponentiation. */
    unsigned r = 1;            /* current remainder, fits in 32 bits */
    unsigned base = 10 % 393;   /* 10 modulo 393 */
    idx_t e = n;

    while (e > 0) {
        if (e & 1) {
            r = (r * base) % 393;
        }
        base = (base * base) % 393;
        e >>= 1;
    }

    /* Multiply by 10 and divide by 393 to get the digit. */
    seq_t rem = (seq_t)r;
    seq_t prod = rem * 10;   /* prod < 3930, safe in seq_t */
    seq_t digit = prod / 393;
    return digit;
}
