/*
 * A010215  Continued fraction for sqrt(167).
 * https://oeis.org/A010215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0) = 12; for n ≥ 1, let e = exponent of 2 in n. a(n) = 1 if e
 *            = 0 (n odd), 11 if e = 1 (n = 2), 24 if e ≥ 2. This matches the
 *            multiplicative definition
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)12;

    /* Count trailing zeros (exponent of 2) */
    idx_t e = 0;
    idx_t m = n;
    while ((m & (idx_t)1) == 0) {
        e++;
        m >>= 1;
    }

    if (e == 0)   return (seq_t)1;
    if (e == 1)   return (seq_t)11;
    return (seq_t)24;
}
