/*
 * A055986  a(n+1) = a(n) converted to base 10 from base 15.
 * https://oeis.org/A055986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0)=10, a(n+1)=interpret a(n) decimal digits as base‑15 and
 *            convert to decimal
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 17, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   convert_base15
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/convert_base15.h"

seq_t a(idx_t n)
{
    seq_t v = (seq_t)10;
    for (idx_t i = 0; i < n; ++i) {
        v = convert_base15(v);
    }
    return v;
}
