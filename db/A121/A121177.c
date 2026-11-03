/*
 * A121177  Catapolyoctagons (see Cyvin et al. for precise definition).
 * https://oeis.org/A121177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n - 5)/10 (for n >= 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* 5^n – 5 is always divisible by 10 for n >= 1 */
    seq_t pow5 = ipow((seq_t)5, (seq_t)n);
    seq_t res  = (pow5 - (seq_t)5) / (seq_t)10;
    return res;
}
