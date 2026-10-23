/*
 * A141364  a(n) = C(n)-1+0^n where C(n) = A000108(n).
 * https://oeis.org/A141364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) - 1 + 0^n (0^0 = 1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */
    seq_t c = catalan(n);          /* C(n) */
    return c - (seq_t)1;           /* a(n) = C(n) - 1 for n > 0 */
}
