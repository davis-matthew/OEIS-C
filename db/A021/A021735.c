/*
 * A021735  Decimal expansion of 1/731.
 * https://oeis.org/A021735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor((10^(n+1) / 731)) mod 10 (the n-th digit after the
 *            decimal point of 1/731)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 10^n mod 731 */
    uint64_t r = 1;
    uint64_t base = 10 % 731;
    uint64_t exp = n;
    while (exp) {
        if (exp & 1ULL)
            r = (r * base) % 731;
        base = (base * base) % 731;
        exp >>= 1;
    }

    /* digit = (r*10)/731 , 0 <= digit <= 9 */
    uint64_t digit = (r * 10) / 731;
    return (seq_t)digit;
}
