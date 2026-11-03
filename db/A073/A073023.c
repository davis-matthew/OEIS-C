/*
 * A073023  Number of solutions to the equation n^x==1 (mod x), 1<=x<=n.
 * https://oeis.org/A073023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{x : 1 ≤ x ≤ n and x divides n^x – 1}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

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
            seq_t r = powmod((seq_t)n, x, (seq_t)x);
            if (r == (seq_t)1)
                count++;
        }
    }
    return count;
}
