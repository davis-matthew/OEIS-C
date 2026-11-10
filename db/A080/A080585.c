/*
 * A080585  Partial sums of A080584.
 * https://oeis.org/A080585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} A080584(i), where A080584 is runs of 3*2^k
 *            zeros then 3*2^k ones, k=0,1,2,
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t start = 0;
    idx_t half = 3;
    idx_t block = 6;

    while (n - start >= block) {
        if (block > (UINT64_MAX >> 1) || start > UINT64_MAX - block)
            break;
        start += block;
        half <<= 1;
        block <<= 1;
    }

    idx_t off = n - start;
    seq_t res = (seq_t)(start / 2);
    if (off >= half)
        res += (seq_t)(off - half + 1);
    return res;
}
