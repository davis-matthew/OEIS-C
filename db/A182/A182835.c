/*
 * A182835  n+floor(sqrt(2n+3)), complement of A179207.
 * https://oeis.org/A182835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+floor(sqrt(2n+3))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    /* 2*n + 3 fits in idx_t (uint64_t) for all requested n. */
    idx_t v = n * 2 + (idx_t)3;

    /* Integer square root of v. */
    idx_t s = isqrt_u64(v);

    /* Result a(n) = n + s. */
    return (seq_t)n + (seq_t)s;
}
