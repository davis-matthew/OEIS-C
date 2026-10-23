/*
 * A034797  a(0) = 0; a(n+1) = a(n) + 2^a(n).
 * https://oeis.org/A034797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2^a(n-1), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t prev = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = pow2((idx_t)prev);   /* 2^prev */
        prev = prev + p;               /* a(i) = a(i-1) + 2^a(i-1) */
    }
    return prev;
}
