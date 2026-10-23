/*
 * A023388  a(n+1) = a(n) converted to base 8 from base 7 (written in base
 *          10).
 * https://oeis.org/A023388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n>=1, a(n+1) = convert the base‑7 representation
 *            of a(n) into base‑8 (read the digits as an octal number)
 * Range:     u8 n <= 25, u16 n <= 38, u32 n <= 50, u64 n <= 60, u128 n <= 71,
 *            big any n
 * Helpers:   convert_base8_from_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/convert_base8_from_base7.h"

/* Main sequence function.  Returns a(n) for n>=1. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0; /* Undefined for offset reasons, but safe. */

    seq_t val = (seq_t)7;       /* a(1) */
    for (idx_t i = 1; i < n; ++i) {
        val = convert_base8_from_base7(val);
    }
    return val;
}
