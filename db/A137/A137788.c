/*
 * A137788  a(n) = 6^n - 5^n - 4^n - 3^n - 2^n.
 * https://oeis.org/A137788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - 5^n - 4^n - 3^n - 2^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 969 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of offset, but harmless */

    seq_t pow6 = ipow_idx((seq_t)6, n);
    seq_t pow5 = ipow_idx((seq_t)5, n);
    seq_t pow4 = ipow_idx((seq_t)4, n);
    seq_t pow3 = ipow_idx((seq_t)3, n);
    seq_t pow2 = ipow_idx((seq_t)2, n);

    return pow6 - pow5 - pow4 - pow3 - pow2;
}
