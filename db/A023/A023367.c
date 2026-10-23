/*
 * A023367  a(n+1) = a(n) converted to base 4 from base 3 (written in base
 *          10).
 * https://oeis.org/A023367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=3, a(n+1)=base3_to_base4(a(n)) where base3_to_base4(v)
 *            interprets v’s base‑3 digits as base‑4 and returns the value in
 *            decimal (base 10)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 17, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   base3_to_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/base3_to_base4.h"

seq_t a(idx_t n)
{
    if (n == 0)             /* undefined for 0; return 0 */
        return (seq_t)0;

    seq_t val = 3;           /* a(1) */
    if (n == 1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
        val = base3_to_base4(val);

    return val;
}
