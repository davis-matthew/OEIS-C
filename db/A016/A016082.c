/*
 * A016082  Add 4, then reverse the decimal digits; start with 10.
 * https://oeis.org/A016082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=10; for n>1, a(n)=rev_decimal(a(n-1)+4)
 * Range:     u8 n <= 6, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)10;
    seq_t val = (seq_t)10;
    for (idx_t i = 2; i <= n; ++i) {
        val += (seq_t)4;          /* add 4 */
        val = reverse_digits(val); /* reverse decimal digits */
    }
    return val;
}
