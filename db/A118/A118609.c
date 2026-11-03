/*
 * A118609  Start with 1 and repeatedly reverse the digits and add 23 to get
 *          the next term.
 * https://oeis.org/A118609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=rev_decimal(a(n-1))+23 */
#include <stdlib.h>
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; undefined below 1 */
        return (seq_t)0;

    seq_t val = (seq_t)1;
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
        val = reverse_digits(val) + (seq_t)23;

    return val;
}
