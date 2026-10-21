/*
 * A219789  Least prime in the form x*y-1 with x > 0, y > 0 and x + y = n > 3.
 * https://oeis.org/A219789
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime of the form x*y-1 with x>0, y>0, x+y=n (n ≥
 *            4)
 * Range:     u8 n <= 32, u16 n <= 797, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                     /* undefined for n < 4 */

    seq_t best = 0;
    for (idx_t x = 1; x < n; ++x)     /* y = n - x */
    {
        idx_t y = n - x;
        seq_t p = (seq_t)x * (seq_t)y - 1;
        if (p < 2)
            continue;
        if (is_prime((idx_t)p))
        {
            if (best == 0 || p < best)
            {
                best = p;
                if (best == 2)           /* can't get smaller than 2 */
                    break;
            }
        }
    }
    return best;
}
