/*
 * A117403  a(n) = Sum_{k=0..floor(n/2)} 2^((n-2*k)*k) for n>=0.
 * https://oeis.org/A117403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^((n-2*k)*k)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 22, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 142 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = (n - 2 * k) * k;
        sum += pow2(exp);
    }
    return sum;
}
