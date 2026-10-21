/*
 * A208658  Row sums of A208657.
 * https://oeis.org/A208658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2*n-1) - n
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, offset starts at 1 */
    seq_t pow = pow2(n - 1);              /* 2^(n‑1) */
    seq_t factor = (seq_t)(2 * (seq_t)n - 1);
    seq_t prod   = seq_mul(pow, factor);  /* (2^(n‑1))*(2*n‑1) */
    seq_t result = prod - (seq_t)n;       /* subtract n */
    return result;
}
