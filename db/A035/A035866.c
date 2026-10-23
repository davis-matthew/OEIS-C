/*
 * A035866  Coordination sequence for A_40 lattice.
 * https://oeis.org/A035866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, for n>=1 a(n)=∑_{d=1}^{40}
 *            C(41,d)·C(n-1,d-1)·C(40-d+n,n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A035866 – coordination sequence for the A_40 lattice) */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Pre‑compute the fixed binomial coefficients C(41,d) */
    static seq_t C41[41];
    static int init = 0;
    if (!init) {
        for (idx_t d = 0; d <= 40; ++d)
            C41[d] = binomial((idx_t)41, d);
        init = 1;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= 40 && d <= n; ++d) {
        seq_t term = C41[d];
        term *= binomial(n - 1, d - 1);           /* C(n-1, d-1) */
        term *= binomial(40 - d + n, n);          /* C(40-d+n, n) */
        sum += term;
    }

    return sum;
}
