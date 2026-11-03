/*
 * A169562  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 21 * 20^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts reduced words in the Coxeter group described in the OEIS entry. */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"
#include <stdint.h>              /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    seq_t pow = ipow_u64_idx(20ULL, n - 1);
    return pow * (seq_t)21;       /* a(n) = 21 * 20^(n-1) */
}
