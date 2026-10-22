/*
 * A003891  Degrees of irreducible representations of group L2(31).
 * https://oeis.org/A003891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   for L2(q) with q = 31 (q == 3 mod 4), the degrees in increasing
 *            order are: 1; (q-1)/2 twice; q-1 repeated (q-3)/4 times; q; q+1
 *            repeated (q-3)/4 times. That gives (q+5)/2 = 18 terms
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t q = 31;
    const idx_t m = (q - 3) / 4;   /* 7 */
    if (n == 1) return (seq_t)1;
    if (n <= 3) return (seq_t)((q - 1) / 2);
    if (n <= 3 + m) return (seq_t)(q - 1);
    if (n == 4 + m) return (seq_t)q;
    if (n <= 4 + 2 * m) return (seq_t)(q + 1);
    return (seq_t)0;
}
