/*
 * A080036  a(n) = n + round(sqrt(2*n)) + 1.
 * https://oeis.org/A080036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n + round(sqrt(2*n)) + 1
 * Range:     u8 n <= 232, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   round_sqrt2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n≥0, with a(0)=1, a(1)=3, … */
#include <stdint.h>
#include "../utils/round_sqrt2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t rs = round_sqrt2(n);
    return nn + rs + (seq_t)1;
}
