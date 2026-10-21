/*
 * A073023  Number of solutions to the equation n^x==1 (mod x), 1<=x<=n.
 * https://oeis.org/A073023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{x : 1 ≤ x ≤ n and x divides n^x – 1}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x)
    {
        if (x == 1)          /* 1 always divides n^1 – 1 */
        {
            count++;
        }
        else
        {
            seq_t r = mod_pow((seq_t)n, x, (seq_t)x);
            if (r == (seq_t)1)
                count++;
        }
    }
    return count;
}
