/*
 * A092187  A092186(n)/2.
 * https://oeis.org/A092187
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 16, u64 n <= 25, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(2) = 1, a(3) = 1,
 *           a(n) = n * a(n-1)          if n is even,
 *           a(n) = ((n+1)/4) * a(n-1)  if n is odd (n≥5)
 *  (The division by 4 is safe because a(n-1) is always divisible by 4 for n≥4.)
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for n < 2 */

    if (n == 2 || n == 3)
        return (seq_t)1;          /* a(2)=1, a(3)=1 */

    seq_t result = (seq_t)1;      /* a(3) = 1 */

    for (idx_t k = 4; k <= n; ++k)
    {
        if ((k & 1) == 0)         /* even k */
        {
            result *= (seq_t)k;
        }
        else                      /* odd k (k ≥ 5) */
        {
            /* a(k-1) is divisible by 4, so divide first to avoid overflow */
            result = (result / (seq_t)4) * ((seq_t)(k + 1));
        }
    }

    return result;
}
