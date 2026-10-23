/*
 * A105597  Central numbers in a Moebius-binomial triangle.
 * https://oeis.org/A105597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(abs(mu(n)),abs(mu(floor(n/2))))
 * Range:     u8 n <= 9486, u16 n <= 9486, u32 n <= 9486, u64 n <= 9486,
 *            u128 n <= 9486, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    bool sq_n   = is_squarefree(n);
    bool sq_half = is_squarefree(n / 2);
    if (!sq_n && sq_half)
        return (seq_t)0;
    return (seq_t)1;
}
