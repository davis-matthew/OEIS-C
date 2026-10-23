/*
 * A181527  Binomial transform of A113127; (1, 1, 3, 7, 15, 31, ...) convolved
 *          with (1, 3, 7, 15, 31, 63, ...).
 * https://oeis.org/A181527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + n + 2^(n+1)*(n-1), a(0)=1, a(1)=4, … (OEIS A181527)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t pow = pow2(n + 1);          /* 2^(n+1)  */
    seq_t res = 3;
    res += (seq_t)n;                  /* add n   */
    res += (seq_t)(n - 1) * pow;      /* add (n-1)*2^(n+1) */
    return res;
}
