/*
 * A023386  a(n+1) = a(n) converted to base 9 from base 6 (written in base
 *          10).
 * https://oeis.org/A023386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 6; for n ≥ 1, a(n+1) is obtained by interpreting the
 *            base‑6 digits of a(n) as a base‑9 number (written in decimal)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 16, u64 n <= 19, u128 n <= 23,
 *            big any n
 * Helpers:   convert_base6_to_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/convert_base6_to_base9.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    seq_t val = (seq_t)6;
    for (idx_t i = 1; i < n; ++i) {
        val = convert_base6_to_base9(val);
    }
    return val;
}
