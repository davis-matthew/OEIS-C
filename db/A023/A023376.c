/*
 * A023376  a(n+1) = a(n) converted to base 8 from base 4 (written in base
 *          10).
 * https://oeis.org/A023376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 4; a(n+1) = a(n) interpreted as base 4 digits written as
 *            a base 8 number (result given in base 10)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Convert a number from base 4 to base 8 treating the same digits. */
static seq_t
convert_base4_to_base8(seq_t x)
{
    seq_t next = 0;
    seq_t mul  = 1;

    while (x > 0) {
        seq_t rem = x % (seq_t)4;   /* digit (0‑3) in base 4 */
        if (rem) next = next + rem * mul;
        mul = mul * (seq_t)8;       /* next power of 8 */
        x   = x / (seq_t)4;         /* shift right by one base‑4 digit */
    }
    return next;
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n<1, safe return */

    seq_t val = (seq_t)4;           /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 1; i < n; ++i) {
        val = convert_base4_to_base8(val);
    }
    return val;
}
