/*
 * A151789  a(1)=1; for n > 1, a(n)=5*4^{wt(n-1)-1}.
 * https://oeis.org/A151789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 5 * 4^(wt(n-1)-1)
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   popcount_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/popcount_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    /* Hamming weight of n-1 */
    int w = popcount_u64(n - 1);
    /* compute 5 * 4^(w-1) */
    idx_t exp = (idx_t)(w > 0 ? w - 1 : 0);
    seq_t p = pow_uint(4ULL, exp);
    return (seq_t)5 * p;
}
