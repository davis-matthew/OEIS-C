/*
 * A081853  Consider recurrence b(0) = (2n+1)/2, b(n) =
 *          b(n-1)*ceiling(b(n-1)); sequence gives first integer reached.
 * https://oeis.org/A081853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ · F^(i-1)(2n+1) where F(x)=x(x+1)/2 and i is the
 *            exponent of 2 in 2n plus 2. Equivalently, i = v₂(n)+2
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   tz64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/tz64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition; no checking needed.  */
    const idx_t n_orig = n;
    const uint64_t tz = tz64(n_orig);       /* v₂(n) */
    const idx_t iter = tz + 1;              /* i‑1 = v₂(n)+1 */

    /* base = 2n + 1, computed in seq_t to avoid overflow */
    seq_t base = ((seq_t)n_orig) * 2 + 1;

    for (idx_t k = 0; k < iter; ++k)
        base = base * (base + 1) / (seq_t)2;   /* apply F */

    return base / (seq_t)2;                    /* final ½ */
}
