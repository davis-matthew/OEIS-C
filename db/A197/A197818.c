/*
 * A197818  Walsh matrix antidiagonals converted to decimal.
 * https://oeis.org/A197818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i=0}^n [popcount(i & (n-i)) is even] * 2^{n-i}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t j = n - i;
        /* parity of popcount of (i & j) */
        if ( ( __builtin_popcountll( (uint64_t)(i & j) ) & 1 ) == 0 ) {
            result += ((seq_t)1) << j;
        }
    }
    return result;
}
