/*
 * A121263  Descending dungeons: see Comments lines for definition.
 * https://oeis.org/A121263
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = 10_(11_(12_(..._(n-1_n)...))) // nested base
 *            interpretation
 * Range:     u8 n <= 23, u16 n <= 30, u32 n <= 40, u64 n <= 55, u128 n <= 79,
 *            big any n
 * Helpers:   decimal_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/decimal_in_base.h"

/* Compute sequence a(n) for n >= 10.
   For n < 10 the result is defined as 0 (unused). */
seq_t a(idx_t n) {
    if (n < 10u) return 0;
    if (n == 10u) return (seq_t)10;
    /* Start with the innermost pair (n-1)_n */
    seq_t val = decimal_in_base(n - 1u, (seq_t)n);
    /* Process all outer levels down to 10_... */
    for (idx_t k = n - 2u; k >= 10u; --k) {
        val = decimal_in_base(k, val);
    }
    return val;
}
