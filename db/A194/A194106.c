/*
 * A194106  Sum{floor(j*sqrt(3)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(3).
 * https://oeis.org/A194106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} ⌊j·√3⌋
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: ⌊j·√3⌋ = ⌊√(3·j²)⌋, so we can use integer square‑root. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t jj = (seq_t)j;      /* j as seq_t */
        seq_t v  = 3 * jj * jj;   /* 3·j², safe because seq_t is big enough for n up to ~10⁶ */
        v = floor_sqrt(v);        /* floor(√(3·j²)) = floor(j·√3) */
        sum += v;
    }
    return sum;
}
