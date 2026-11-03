/*
 * A105597  Central numbers in a Moebius-binomial triangle.
 * https://oeis.org/A105597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(abs(mu(n)),abs(mu(floor(n/2))))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9486 (2026-11-01)
 * Helpers:   is_squarefree_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree_idx.h"

seq_t a(idx_t n)
{
    bool sq_n   = is_squarefree_idx(n);
    bool sq_half = is_squarefree_idx(n / 2);
    if (!sq_n && sq_half)
        return (seq_t)0;
    return (seq_t)1;
}
