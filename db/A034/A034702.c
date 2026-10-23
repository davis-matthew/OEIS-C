/*
 * A034702  a(n+1) is the smallest number not of the form a(i), a(i) + a(n-1),
 *          or |a(i) - a(n-1)|.
 * https://oeis.org/A034702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1 * a(n) = 2 for n = 2 * a(n) = 3*n + 3 -
 *            2*2^floor(log2(n+2)) for n > 2 * This implements the closed form
 *            derived from the OEIS entry. * No other global state is required
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;
    if (n == (idx_t)2)
        return (seq_t)2;

    /* For n > 2, use the formula */
    idx_t m = n + (idx_t)2;
    unsigned k = floor_log2_uint64(m);       /* floor(log2(n+2)) */
    seq_t p2 = pow2((idx_t)k);              /* 2^k  as seq_t */

    seq_t term = (seq_t)3 * (seq_t)n;        /* 3*n          */
    term += (seq_t)3;                       /* 3*n + 3      */
    term -= (p2 + p2);                      /* subtract 2*2^k */

    return term;
}
