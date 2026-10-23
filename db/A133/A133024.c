/*
 * A133024  Divisors of 8128, the 4th perfect number.
 * https://oeis.org/A133024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) = 2^(n-1) for 1 <= n <= 7; a(n) = 2^(n-1) - 2^(n-8) for 8
 *            <= n <= 14. For n > 14 the sequence is undefined, so return 0
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard against underflow */
    if (n < 8) {
        return pow2(n - 1);
    } else if (n <= 14) {
        return pow2(n - 1) - pow2(n - 8);
    } else {
        return 0;                  /* undefined beyond known divisors */
    }
}
