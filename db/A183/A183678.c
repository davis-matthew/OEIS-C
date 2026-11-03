/*
 * A183678  Number of (n+1) X 6 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 434*7^n + 3612*6^n + 13510*5^n + 32648*4^n +
 *            58506*3^n + 79924*2^n + 73502
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 963 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow2 = ipow_u64_idx(2, n);
    seq_t pow3 = ipow_u64_idx(3, n);
    seq_t pow4 = ipow_u64_idx(4, n);
    seq_t pow5 = ipow_u64_idx(5, n);
    seq_t pow6 = ipow_u64_idx(6, n);
    seq_t pow7 = ipow_u64_idx(7, n);
    seq_t pow8 = ipow_u64_idx(8, n);

    seq_t result = 73502;
    result += 79924 * pow2;
    result += 58506 * pow3;
    result += 32648 * pow4;
    result += 13510 * pow5;
    result += 3612 * pow6;
    result += 434 * pow7;
    result += 8 * pow8;
    return result;
}
