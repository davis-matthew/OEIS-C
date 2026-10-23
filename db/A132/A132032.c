/*
 * A132032  Product{0<=k<=floor(log_8(n)), floor(n/8^k)}, n>=1.
 * https://oeis.org/A132032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0}^{⌊log_8 n⌋} ⌊n / 8^k⌋ * Computes the product of
 *            successive floor divisions by 8. * n is indexed from 1 (offset
 *            1)
 * Range:     u8 n <= 47, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of sequence range, safe default */
    seq_t result = 1;
    idx_t tmp = n;
    while (tmp > 0)
    {
        result *= (seq_t)tmp;           /* multiply by current floor division */
        tmp /= 8;                       /* next division by 8 */
    }
    return result;
}
