/*
 * A115384  Partial sums of Thue-Morse numbers A010060.
 * https://oeis.org/A115384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0}^n A010060(k)^2 (A010060(k) is the parity of the
 *            binary weight of k. Hence a(n) equals the number of k ≤ n with
 *            odd binary weight.)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;                /* cumulative number of odd‑weight numbers */
    idx_t  parity = 0;              /* parity of the prefix bits processed so far */

    /* iterate over bits of n from most significant to least significant */
    for (int i = 63; i >= 0; --i)
    {
        idx_t bit = ((idx_t)1) << i;
        if (bit > n) continue;     /* current bit not present in n */

        if (n & bit)               /* the bit of n is 1 */
        {
            if (i == 0)            /* lower part has no bits */
            {
                if (parity)        /* the number with this prefix has odd parity */
                    count += 1;
            }
            else
            {
                /* numbers with this bit 0 and all lower bits arbitrary:
                   half of them have odd parity */
                count += (seq_t)((1ULL) << (i - 1));
            }
            parity ^= 1;           /* we choose the bit 1 for the remaining suffix */
        }
    }

    /* include n itself, whose parity is now stored in 'parity' */
    if (parity) count += 1;

    return count;
}
