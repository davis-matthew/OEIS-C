/*
 * A066308  a(n) = (sum of digits of n) * (product of digits of n).
 * https://oeis.org/A066308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (sum of decimal digits of n) * (product of decimal digits
 *            of n)
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1, but guard against 0 */

    uint64_t tmp   = n;
    uint64_t sum   = 0;
    uint64_t prod  = 1;

    while (tmp != 0) {
        uint64_t digit = tmp % 10;
        sum  += digit;
        prod *= digit;
        tmp  /= 10;
    }

    return (seq_t)sum * (seq_t)prod;
}
