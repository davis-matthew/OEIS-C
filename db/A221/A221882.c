/*
 * A221882  Number of order-preserving or order-reversing full contraction
 *          mappings of an n-chain.
 * https://oeis.org/A221882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*2^(n-1) - n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 1, guard against wrap‑around */
    seq_t pow = pow2(n - 1ULL);          /* 2^(n-1) */
    return ((seq_t)(n + 1) * pow) - n;
}
