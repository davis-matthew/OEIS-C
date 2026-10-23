/*
 * A021247  Decimal expansion of 1/243.
 * https://oeis.org/A021247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/243) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 243 */
    uint64_t r = 1;
    uint64_t base = 10 % 243;
    idx_t exp = n;
    while (exp) {
        if (exp & 1)
            r = (r * base) % 243;
        base = (base * base) % 243;
        exp >>= 1;
    }
    /* The digit is floor(10*r / 243); r < 243 */
    return (seq_t)((10 * r) / 243);
}
