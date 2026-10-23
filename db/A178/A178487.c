/*
 * A178487  a(n) = floor(n^(1/5)): integer part of fifth root of n.
 * https://oeis.org/A178487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(1/5)) – the integer part of the fifth root of n
 * Range:     u8 n <= 31, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 0 has a trivial answer */
    if (n == 0)
        return 0;

    /* n as a seq_t for comparisons with powers */
    const seq_t n_s = (seq_t)n;

    /* Find an upper bound hi such that hi^5 > n */
    idx_t lo = 0;
    idx_t hi = 1;
    while (pow_uint(hi, 5) <= n_s) {
        /* Avoid overflow of hi – if it cannot be doubled we stop */
        if (hi > UINT64_MAX / 2)
            break;
        hi <<= 1;            /* hi *= 2 */
    }
    /* If the last doubling still did not exceed n, grow hi a bit more */
    while (pow_uint(hi, 5) <= n_s) {
        /* In practice this loop runs at most once because hi^5 grows very fast */
        if (hi == UINT64_MAX)
            break;
        hi += 1;
    }

    /* Binary search for the largest k with k^5 <= n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (pow_uint((uint64_t)mid, 5) <= n_s)
            lo = mid;
        else
            hi = mid - 1;
    }

    return (seq_t)lo;
}
