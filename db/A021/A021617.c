/*
 * A021617  Decimal expansion of 1/613.
 * https://oeis.org/A021617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10 * (10^n mod 613)) / 613)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Compute r = 10^n mod 613 using binary exponentiation. */
    uint64_t base = 10 % 613;      /* base < 613 */
    uint64_t result = 1;           /* result < 613 */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 613;
        }
        base = (base * base) % 613;
        exp >>= 1;
    }
    /* Now result = 10^n mod 613 */
    seq_t rem = (seq_t)result;          /* cast to seq_t; value < 613 */
    seq_t digit = (rem * 10) / 613;    /* integer division, exact digit */
    return digit;
}
