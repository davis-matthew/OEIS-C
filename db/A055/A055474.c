/*
 * A055474  Powers of ten written in base 3.
 * https://oeis.org/A055474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 462 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function to compute OEIS sequence A055474: Powers of ten written in base 3.
   a(n) = decimal integer composed of the base‑3 digits of 10^n. */
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t x = pow10(n);           /* x = 10^n  */
    seq_t result = 0;             /* accumulator for decimal digits  */
    seq_t pow10_dec = 1;          /* 10^k in decimal (k = processed digits) */
    while (x != 0)
    {
        /* least significant base‑3 digit */
        int d = (int)(x % 3);
        /* append digit d to the decimal representation */
        result = result + (seq_t)d * pow10_dec;
        /* advance decimal power of 10 */
        pow10_dec *= 10;
        /* integer division by 3 */
        x /= 3;
    }
    return result;
}
