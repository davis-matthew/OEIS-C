/*
 * A229358  Total sum of n-th powers of parts in all partitions of 7.
 * https://oeis.org/A229358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30 + 11*2^n + 6*3^n + 3*4^n + 2*5^n + 6^n + 7^n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 488 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)30;

    seq_t pow2 = ipow_idx((seq_t)2, n);
    seq_t pow3 = ipow_idx((seq_t)3, n);
    seq_t pow4 = ipow_idx((seq_t)4, n);
    seq_t pow5 = ipow_idx((seq_t)5, n);
    seq_t pow6 = ipow_idx((seq_t)6, n);
    seq_t pow7 = ipow_idx((seq_t)7, n);

    result += (seq_t)11 * pow2;
    result += (seq_t)6  * pow3;
    result += (seq_t)3  * pow4;
    result += (seq_t)2  * pow5;
    result += pow6;
    result += pow7;

    return result;
}
