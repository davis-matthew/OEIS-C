/*
 * A234436  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 1
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32 * 14^((n-1)/2) for odd n, a(n) = 120 * 14^((n-2)/2)
 *            for even n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */
    seq_t base = (n & 1) ? (seq_t)32 : (seq_t)120;
    idx_t e = (n & 1) ? ((n - 1) / 2) : ((n - 2) / 2);
    seq_t pow = seq_pow((seq_t)14, e);
    return base * pow;
}
