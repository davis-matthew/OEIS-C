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
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts reduced words in the Coxeter group described in the OEIS entry. */

#include "../utils/pow_uint.h"   /* provides pow_uint(uint64_t, idx_t) */
#include <stdint.h>              /* for uint64_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */
    seq_t pow = pow_uint(20ULL, n - 1);
    return pow * (seq_t)21;       /* a(n) = 21 * 20^(n-1) */
}
