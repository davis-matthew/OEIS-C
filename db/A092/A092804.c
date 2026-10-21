/*
 * A092804  Expansion of (1+10x)/((1-1000x^3)).
 * https://oeis.org/A092804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n mod 3 = 2, otherwise a(n) = 10^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /*  n is unsigned, %3 works as expected */
    if ((n % 3) == 2) {
        return (seq_t)0;
    }
    return pow10(n);   /* pow10(e) returns the integer 10^e in seq_t form */
}
