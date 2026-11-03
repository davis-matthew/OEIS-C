/*
 * A141364  a(n) = C(n)-1+0^n where C(n) = A000108(n).
 * https://oeis.org/A141364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) - 1 + 0^n (0^0 = 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */
    seq_t c = catalan(n);          /* C(n) */
    return c - (seq_t)1;           /* a(n) = C(n) - 1 for n > 0 */
}
