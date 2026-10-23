/*
 * A115134  Third diagonal sequence of triangle A115127.
 * https://oeis.org/A115134
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) - Catalan(n-1) + Catalan(n-3) (n >= 4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;   /* undefined for n < 4 */
    seq_t cn   = catalan(n);
    seq_t cn1  = catalan(n - 1);
    seq_t cn3  = catalan(n - 3);
    return cn - cn1 + cn3;
}
