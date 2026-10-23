/*
 * A182697  a(n) is the smallest 5-digit number with exactly n divisors, or
 *          a(n) = 0 if no such number exists.
 * https://oeis.org/A182697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest 5‑digit integer with exactly n divisors, or
 *            0 if no such integer exists. The sequence is computed by
 *            scanning all five‑digit numbers once per program run
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t table[129];   /* 0…128, table[0] unused */

    if (!init) {
        for (int i = 0; i < 129; ++i) table[i] = 0;

        for (uint64_t x = 10000; x <= 99999; ++x) {
            idx_t d = (idx_t)divisor_count_u64(x);
            if (d <= 128 && d > 0) {
                /* store the smallest five‑digit number for this divisor count */
                if (table[d] == 0 || (uint64_t)table[d] > x)
                    table[d] = (seq_t)x;
            }
        }
        init = true;
    }

    if (n > 128) return 0;
    return table[n];
}
