/*
 * A136796  Number of labeled marked rooted trees with n nodes.
 * https://oeis.org/A136796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) * n^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 188 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(0) is undefined but
       we return 0 for defensive safety. */
    if (n == 0)
        return (seq_t)0;

    seq_t pow2 = ipow((seq_t)2, (seq_t)(2 * n - 1));
    seq_t pown = ipow((seq_t)n, (seq_t)(n - 1));

    return pow2 * pown;
}
