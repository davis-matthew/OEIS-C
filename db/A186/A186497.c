/*
 * A186497  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=3i-2 and g(j)=j-th triangular
 *          number. Complement of A186498.
 * https://oeis.org/A186497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + #{j ≥ 1 : T_j < 3n-2}, where T_j = j(j+1)/2.
 *            Equivalently, let v = 3n-2, D = 1+8v, s = ⌊√D⌋, j = ⌊(s-1)/2⌋.
 *            If T_j ≥ v then j--. Then a(n) = n + j
 * Range:     u8 n <= 3, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset=1, a(0) is undefined */

    seq_t n_seq = (seq_t)n;
    seq_t v = 3 * n_seq - 2;            /* f(n) = 3n-2 */
    seq_t D = 8 * v + 1;                /* 1+8v for triangl. test */

    seq_t sqrtD = floor_sqrt(D);        /* integer sqrt( D ) */
    seq_t j = (sqrtD - 1) / 2;          /* candidate count of T_j < v */

    seq_t Tj = j * (j + 1) / 2;         /* T_j = j(j+1)/2 */
    if (Tj >= v)                        /* adjust if v itself is triangular */
        j--;

    return n_seq + j;                   /* rank of f(n) in the joint list */
}
