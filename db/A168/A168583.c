/*
 * A168583  The number of ways of partitioning the multiset {1,1,2,3,...,n-1}
 *          into exactly three nonempty parts.
 * https://oeis.org/A168583
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-2) - 3*2^(n-3) + 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 29 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)3)
        return (seq_t)0;

    seq_t pow3 = ipow((seq_t)3, (seq_t)(n - (idx_t)2));
    seq_t pow2 = ipow((seq_t)2, (seq_t)(n - (idx_t)3));

    return pow3 - (seq_t)3 * pow2 + (seq_t)1;
}
