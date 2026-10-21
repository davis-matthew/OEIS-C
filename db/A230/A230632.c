/*
 * A230632  Number of integers m such that m + (sum of digits in base-4
 *          representation of m) = n.
 * https://oeis.org/A230632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ m ≥ 0 | m + sumdigits4(m) = n } (sumdigits4(m) = sum
 *            of digits of m in base‑4)
 * Range:     u8 n <= 665, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sumdigits4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sumdigits4.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t m = 0; m <= n; ++m) {
        seq_t val = (seq_t)m + sumdigits4(m);   /* m + digit sum */
        if (val == (seq_t)n)
            ++cnt;
    }
    return cnt;
}
