/*
 * A147631  a(n) = Product_{k = 1..n-1} (9*k - 2).
 * https://oeis.org/A147631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n-1} (9*k - 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        seq_t term = (seq_t)9 * (seq_t)i - (seq_t)2;
        result = result * term;
    }
    return result;
}
