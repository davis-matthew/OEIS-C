/*
 * A035854  Coordination sequence for A_28 lattice.
 * https://oeis.org/A035854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Sum_{d=1..28} C(29,d) · C(n-1,d-1) · C(28-d+n,n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* a(0) is defined to be 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t half = n;          /* m = 2n, so m/2 = n */
    seq_t total = (seq_t)0;

    for (idx_t d = 1; d <= 28; ++d) {
        seq_t t1 = binomial((idx_t)29, d);          /* C(29,d) */
        seq_t t2 = binomial(half - 1, d - 1);      /* C(n-1,d-1) */
        seq_t t3 = binomial(28 - d + half, half); /* C(28-d+n,n) */
        total += t1 * t2 * t3;
    }

    return total;
}
