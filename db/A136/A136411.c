/*
 * A136411  a(n) = Product_{k=1..n} (2*k-1)^(2*n-2*k+1).
 * https://oeis.org/A136411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Prod_{k=1..n} (2k-1)^(2n-2k+1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1; unused for n=0 */
    seq_t result = 1;
    idx_t limit = (n << 1) - 1;           /* 2n - 1 */
    for (idx_t odd = 1; odd <= limit; odd += 2) {
        seq_t exp = (seq_t)(limit - odd + 1); /* 2n - odd */
        result *= big_pow((seq_t)odd, exp);
    }
    return result;
}
