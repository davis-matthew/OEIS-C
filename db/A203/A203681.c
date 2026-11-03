/*
 * A203681  Vandermonde sequence using x^2 - xy + y^2 applied to
 *          (1,2,4,...,2^(n-1)).
 * https://oeis.org/A203681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{(n-1)n(n-2)/3} * ∏_{d=1}^{n-1} (1 - 2^d +
 *            2^{2d})^{\,n-d}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-01)
 * Helpers:   ipow_idx, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1, but guard against 0 */

    seq_t prod = 1;
    for (idx_t d = 1; d < n; ++d)
    {
        seq_t pow2d = ipow_u64_idx(2, d);                     /* 2^d   */
        seq_t base  = pow2d * pow2d - pow2d + 1;          /* (2^d)^2 - 2^d + 1 */
        prod *= ipow_idx(base, n - d);                    /* base^{n-d} */
    }

    /* exponent of 2: (n-1)*n*(n-2)/3 */
    idx_t exp2 = ((n - 1) * n * (n - 2)) / 3;
    seq_t two_pow = ipow_u64_idx(2, exp2);

    prod *= two_pow;
    return prod;
}
