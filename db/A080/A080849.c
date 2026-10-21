/*
 * A080849  a(n) = (mu(n)+2)*n^2, where mu is the Moebius function (A008683).
 * https://oeis.org/A080849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (mu(n)+2)*n^2
 * Range:     u8 n <= 9, u16 n <= 154, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   mobius_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where mu(n) is the Möbius function. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mobius_of.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined by the OEIS definition; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    int mu = mobius_of(n);
    seq_t cn = (seq_t)n;
    seq_t n_sq = cn * cn;
    seq_t factor = (seq_t)(mu + 2);   /* mu+2 is 1,2, or 3 */
    return n_sq * factor;
}
