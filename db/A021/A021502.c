/*
 * A021502  Decimal expansion of 1/498.
 * https://oeis.org/A021502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/498 * Recurrence:
 *            a(n) = floor(((1 * 10^n) mod 498) * 10 / 498)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  // for uint64_t definition

seq_t a(idx_t n) {
    const seq_t DEN = 498;          // denominator of 1/498
    seq_t r = 1;                    // current remainder
    for (idx_t i = 0; i < n; ++i) {
        r = (r * (seq_t)10) % DEN;  // advance remainder by one decimal place
    }
    return (r * (seq_t)10) / DEN;   // digit at position n
}
