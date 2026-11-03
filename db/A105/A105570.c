/*
 * A105570  Nonsquarefree numbers in place: a(n) = n if n is not squarefree, 0
 *          otherwise.
 * https://oeis.org/A105570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n if n is not squarefree, 0 otherwise
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 106 terms through n = 9868 (2026-11-01)
 * Helpers:   is_squarefree_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (is_squarefree_idx(n)) return 0;
    return (seq_t)n;
}
