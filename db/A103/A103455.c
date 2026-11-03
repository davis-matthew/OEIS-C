/*
 * A103455  a(n) = 0^n + 5^n - 1.
 * https://oeis.org/A103455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 5^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 376 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t pow5 = ipow_idx((seq_t)5, n);
    return pow5 - 1;
}
