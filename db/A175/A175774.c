/*
 * A175774  a(n) = 7*(10^n-5).
 * https://oeis.org/A175774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*(10^n - 5). Recurrence: a(1)=35, a(n)=10*a(n-1)+315
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* 10^n is computed via the utility function pow10.
       The result fits in seq_t for the required precision.     */
    seq_t p = pow10(n);        /* p = 10^n   */
    seq_t res = 7 * (p - 5);   /* a(n) = 7*(10^n - 5) */
    return res;
}
