/*
 * A092477  Triangle read by rows: T(n,k) = (2^k - 1)^n, 1<=k<=n.
 * https://oeis.org/A092477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2^k - 1)^m, where m = ceil((√(1+8n) - 1)/2), k = n -
 *            m(m-1)/2 (indexed with a(1) = first element of the triangle)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find the row m such that m*(m+1)/2 >= n  */
    idx_t m = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++m;
        cum += m;
    }
    /* column k within the row */
    idx_t k = n - (cum - m);

    /* compute (2^k - 1)^m */
    seq_t base = pow2(k) - 1;       /* 2^k - 1 */
    return ipow_idx(base, m);        /* raise to the power m */
}
