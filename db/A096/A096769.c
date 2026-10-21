/*
 * A096769  a(n)=Max{ (i+j)!/i!^2 | 0<=i,j<=n }.
 * https://oeis.org/A096769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product of the first n-1 positive integers ≥ 2 that are
 *            not perfect squares. a(1) = 1 (empty product). This follows from
 *            the OEIS definition of A096769
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_square.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t prod = (seq_t)1;
    idx_t  done = 0;

    for (idx_t m = 2; ; ++m)
    {
        if (is_square(m)) continue;          /* skip perfect squares */
        prod *= (seq_t)m;
        if (++done == n - 1) break;
    }
    return prod;
}
