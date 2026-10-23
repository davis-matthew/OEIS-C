/*
 * A021720  Decimal expansion of 1/716.
 * https://oeis.org/A021720
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

/* Digit of 1/716 at index n after the decimal point.
   a(n) = floor( (10 * (10^n mod 716)) / 716 )
*/
#include <stdint.h>

seq_t a(idx_t n) {
    const uint64_t mod = 716;
    uint64_t r = 1;         /* r = 10^n mod 716 */
    uint64_t base = 10;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U) r = (r * base) % mod;
        base = (base * base) % mod;
        exp >>= 1U;
    }
    /* digit = (r * 10) / 716 */
    uint64_t digit = (r * 10ULL) / mod;
    return (seq_t)digit;
}
