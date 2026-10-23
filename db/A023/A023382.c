/*
 * A023382  a(n+1) = a(n) converted to base 9 from base 5 (written in base
 *          10).
 * https://oeis.org/A023382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5; for n ≥ 1, a(n+1) = (a(n) expressed in base‑5)
 *            interpreted as a base‑9 number. This implements a(n) = a(n)
 *            converted to base 9 from base 5
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 11, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   base5_to_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/base5_to_base9.h"

/* Public sequence generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* behaviour for n < 1 is unspecified */

    seq_t val = 5;               /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        val = base5_to_base9(val);
    }
    return val;
}
