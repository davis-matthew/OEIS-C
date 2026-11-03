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
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^exp mod 305 using binary exponentiation.
   exp is idx_t (unsigned 64‑bit). */
static inline uint64_t modpow10_u(idx_t exp) {
    uint64_t result = 1;
    uint64_t base   = 10 % 305;
    while (exp) {
        if (exp & 1) result = (result * base) % 305;
        base = (base * base) % 305;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t r = modpow10_u(n);           /* r = 10^n mod 305   */
    uint64_t d = (r * 10) / 305;          /* digit extraction   */
    return (seq_t)d;                      /* return as seq_t    */
}
