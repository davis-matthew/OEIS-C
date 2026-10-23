/*
 * A104971  Numbers with 4 distinct digits {1,2,3,4} such that all adjacent
 *          digits(as well as first and last digits) are coprime.
 * https://oeis.org/A104971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, base, fini
 * Formula:   Enumerates all 4‑digit numbers with distinct digits chosen from
 *            {1,2,3,4} that satisfy the adjacency coprimality condition (i.e.
 *            the digits 2 and 4 must not be adjacent in the circular cycle).
 *            The sequence thus consists of the eight numbers listed in OEIS
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_int.h"

static seq_t seq_arr[8];
static idx_t seq_len = 0;

static void generate_seq(void)
{
    if (seq_len != 0)
        return;
    idx_t pos = 0;
    for (int d0 = 1; d0 <= 4; ++d0)
    {
        for (int d1 = 1; d1 <= 4; ++d1)
        {
            if (d1 == d0)
                continue;
            if (gcd_int(d0, d1) != 1)
                continue;
            for (int d2 = 1; d2 <= 4; ++d2)
            {
                if (d2 == d0 || d2 == d1)
                    continue;
                if (gcd_int(d1, d2) != 1)
                    continue;
                for (int d3 = 1; d3 <= 4; ++d3)
                {
                    if (d3 == d0 || d3 == d1 || d3 == d2)
                        continue;
                    if (gcd_int(d2, d3) != 1)
                        continue;
                    if (gcd_int(d3, d0) != 1)
                        continue;
                    seq_arr[pos++] = (seq_t)(d0 * 1000 + d1 * 100 + d2 * 10 + d3);
                }
            }
        }
    }
    seq_len = pos;
}

seq_t a(idx_t n)
{
    if (seq_len == 0)
        generate_seq();
    if (n < 1 || n > seq_len)
        return 0;
    return seq_arr[n - 1];
}
