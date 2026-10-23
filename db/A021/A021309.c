/*
 * A021309  Decimal expansion of 1/305.
 * https://oeis.org/A021309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ( (10^n mod 305)*10 ) / 305 This yields the nth digit
 *            (starting at n = 0) of the decimal expansion of 1/305
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   modpow10_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/modpow10_u.h"

seq_t a(idx_t n) {
    uint64_t r = modpow10_u(n);           /* r = 10^n mod 305   */
    uint64_t d = (r * 10) / 305;          /* digit extraction   */
    return (seq_t)d;                      /* return as seq_t    */
}
