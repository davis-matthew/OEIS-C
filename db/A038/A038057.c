/*
 * A038057  a(n) = 2^n*n^(n-1).
 * https://oeis.org/A038057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n*n^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1; offset starts at 1 */
    seq_t p2 = pow2(n);                 /* 2^n */
    seq_t pn = pow_int((seq_t)n, n - 1); /* n^(n-1) */
    return p2 * pn;                     /* a(n) = 2^n * n^(n-1) */
}
