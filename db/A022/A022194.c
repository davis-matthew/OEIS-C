/*
 * A022194  Gaussian binomial coefficients [n, 11] for q = 2.
 * https://oeis.org/A022194
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Gaussian binomial coefficient [n,11] for q = 2
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 13, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ∏_{i=1..11} (2^(n-i+1)-1)/(2^i-1). */
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(n) is defined only for n ≥ 11.  Return 0 for smaller n. */
    if (n < 11)
        return 0;

    /* Use a simple dynamic‑programming recurrence:
       [m,k] = [m-1,k] + 2^(m-k) * [m-1,k-1]  (q = 2).  */
    seq_t prev[12];
    seq_t curr[12];

    /* initialise row for m = 0:  [0,0] = 1, all other entries 0 */
    prev[0] = 1;
    for (idx_t i = 1; i <= 11; ++i)
        prev[i] = 0;

    for (idx_t m = 1; m <= n; ++m) {
        curr[0] = 1;                  /* [m,0] = 1 for all m */
        for (idx_t k = 1; k <= 11; ++k) {
            seq_t addend = 0;
            if (m >= k)               /* 2^(m-k) is only defined for non‑negative exponents */
                addend = pow2(m - k) * prev[k - 1];
            curr[k] = prev[k] + addend;
        }
        /* copy current row to prev for next iteration */
        for (idx_t i = 0; i <= 11; ++i)
            prev[i] = curr[i];
    }

    /* prev[11] now holds the desired Gaussian binomial coefficient. */
    return prev[11];
}
