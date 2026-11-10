/*
 * A228132  First differences of A014311.
 * https://oeis.org/A228132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A014311(n+1) - A014311(n), where A014311 lists the
 *            numbers with exactly three 1's in binary
 * Range:     u8 n <= 164, u16 n <= 968, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 990 (2026-11-07)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t A014311_a(idx_t n)
{
    for (idx_t a = 2; ; ++a)
    {
        idx_t count = (a + 1) * a * (a - 1) / 6;

        if (count >= n)
        {
            idx_t prev = 0;
            if (a >= 3)
                prev = a * (a - 1) * (a - 2) / 6;

            idx_t offset = n - prev;
            idx_t q      = offset - 1;

            idx_t b;
            for (b = 1; ; ++b)
                if (b * (b + 1) / 2 > q)
                    break;

            idx_t cumulative_prev = (b - 1) * b / 2;
            idx_t c = q - cumulative_prev;

            return pow2(a) + pow2(b) + pow2(c);
        }
    }
}

seq_t a(idx_t n)
{
    return A014311_a(n + 1) - A014311_a(n);
}
