/*
 * A083825  a(1) = 12; then numbers obtained at every stage of division by 9
 *          in the following process: multiply by 9, reverse the digits,
 *          divide by 9, reverse the digits, multiply by 9, reverse the
 *          digits, divide by 9, ...
 * https://oeis.org/A083825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow10, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  Starting from 12, repeatedly multiply by 9, reverse decimal digits,
   divide by 9, and record the result after each division.  For n > 16 the
   sequence follows the pattern
        a(n) = 10^(n-15) + 2   if n is even
        a(n) = 10^(n-15) + 9   if n is odd. */
#include <stdint.h>
#include "../utils/reverse_decimal.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)12;

    if (n <= 16) {
        seq_t cur = (seq_t)12;
        for (idx_t i = 2; i <= n; ++i) {
            cur = cur * (seq_t)9;
            cur = reverse_decimal(cur);
            cur = cur / (seq_t)9;           /* result of division by 9 */
            if (i == n) return cur;
            cur = reverse_decimal(cur);     /* prepare for next cycle */
        }
        return cur;  /* unreachable */
    } else {
        seq_t pow = pow10(n - 15);
        if ((n & 1) == 0) {                  /* even n */
            return pow + (seq_t)2;
        } else {                             /* odd n */
            return pow + (seq_t)9;
        }
    }
}
