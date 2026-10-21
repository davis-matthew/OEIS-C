/*
 * A063063  'Reverse and Add!' trajectory of 10563.
 * https://oeis.org/A063063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 10563, a(n) = a(n‑1) + rev_decimal(a(n‑1)), for n ≥ 1
 *            where rev_decimal(v) is the decimal digit reversal of v
 * Range:     u8 none, u16 n <= 1, u32 n <= 11, u64 n <= 31, u128 n <= 72,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev_decimal.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t value = (seq_t)10563;
    for (idx_t i = 1; i <= n; ++i) {
        value = value + rev_decimal(value);
    }
    return value;
}
