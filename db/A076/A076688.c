/*
 * A076688  a(n) = 2^(2^n-n-2).
 * https://oeis.org/A076688
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2^n - n - 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 10 terms through n = 11 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For n < 2 we return 0 */
    if (n < 2) return (seq_t)0;

    /* Compute e = 2^n  */
    seq_t e = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        e *= (seq_t)2;          /* e = e * 2  */

    /* e = 2^n - n - 2  */
    e -= (seq_t)n;
    e -= (seq_t)2;

    /* Return 2^e  */
    return ipow((seq_t)2, e);
}
