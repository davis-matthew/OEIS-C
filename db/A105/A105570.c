/*
 * A105570  Nonsquarefree numbers in place: a(n) = n if n is not squarefree, 0
 *          otherwise.
 * https://oeis.org/A105570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n if n is not squarefree, 0 otherwise
 * Range:     u8 n <= 255, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (is_squarefree(n)) return 0;
    return (seq_t)n;
}
