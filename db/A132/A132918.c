/*
 * A132918  Identity matrix with interpolated zeros.
 * https://oeis.org/A132918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is a hexagonal number (1,6,15,28,45,66,...);
 *            otherwise 0. An integer n is hexagonal iff 8 n + 1 is a perfect
 *            square and that square ≡ 3 (mod 4)
 * Range:     u8 n <= 9086, u16 n <= 9086, u32 n <= 9086, u64 n <= 9086,
 *            u128 n <= 9086, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n == 0 to be safe. */
    if (n == 0)
        return (seq_t)0;

    /* Compute m = 8*n + 1 using a wide enough type (idx_t). */
    idx_t m = n * 8 + 1;

    /* Integer square root of m.  The helper returns floor(sqrt(m)). */
    idx_t s = _floor_isqrt_u64(m);

    /* Test for perfect square and the (s + 1) % 4 == 0 condition. */
    if (s * s == m && (s + 1) % 4 == 0)
        return (seq_t)1;           /* hexagonal number */
    else
        return (seq_t)0;           /* not a hexagonal number */
}
