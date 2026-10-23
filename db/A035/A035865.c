/*
 * A035865  Coordination sequence for A_39 lattice.
 * https://oeis.org/A035865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..39} C(40,d) * C(n-1,d-1) * C(39-d+n,n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (where n = m/2, m is the norm, which is always even) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 1560, ... */
    if (n == 0)
        return 1;

    seq_t result = 0;
    idx_t k = n;                     /* k = n  (i.e., m/2) */

    for (idx_t d = 1; d <= 39; ++d) {
        if (d > k)               /* C(k-1,d-1) is zero otherwise */
            break;

        seq_t c1 = binomial(40, d);
        seq_t c2 = binomial(k - 1, d - 1);
        idx_t new_n = 39 - d + k; /* argument for the third binomial */
        seq_t c3 = binomial(new_n, k);

        seq_t term = c1 * c2;
        term = term * c3;
        result += term;
    }

    return result;
}
