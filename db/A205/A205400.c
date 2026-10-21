/*
 * A205400  Ordered differences of quarter-squares.
 * https://oeis.org/A205400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s(i) - s(j) where * s(k) = floor((k+1)^2 / 4), * i is the
 *            smallest integer with i(i‑1)/2 ≥ n, * j = n - (i‑1)(i‑2)/2 (1 ≤
 *            j < i)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   sqrt_floor128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor128.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  */
    /* Find i such that i(i‑1)/2 ≥ n.  */
    unsigned __int128 v = (unsigned __int128)8 * n + 1;     /* 1 + 8*n  */
    unsigned long long r = sqrt_floor128(v);              /* floor(sqrt)  */
    idx_t i = (idx_t)((r + 1) >> 1);                     /* (1 + r)/2   */
    /* Correct for possible rounding down.  */
    if ( (unsigned __int128)i * (i - 1) / 2 < n )
        i += 1;

    /* j = n - C(i‑1) where C(m) = m(m+1)/2 = (m+1)m/2 */
    idx_t prevC = (idx_t)((unsigned __int128)(i - 1) * (i - 2) / 2);
    idx_t j = n - prevC;                      /* 1 ≤ j < i */

    /* Compute s(i) and s(j).  */
    seq_t si = ((seq_t)(i + 1) * (seq_t)(i + 1)) / 4;
    seq_t sj = ((seq_t)(j + 1) * (seq_t)(j + 1)) / 4;

    return si - sj;
}
