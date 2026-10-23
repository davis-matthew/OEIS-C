/*
 * A189573  Positions of 0 in A189572; complement of A080652 (conjectured).
 * https://oeis.org/A189573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(α·n + β) where α = 1 + √2 / 2 and β = 1/2 – √2 / 2
 *            The sequence can be written as a(n) = n + floor( ( (n‑1)·√2 ) /
 *            2 + 1/2 ) = n + ( floor( (n‑1)·√2 ) + 1 ) / 2 thus only integer
 *            arithmetic is required
 * Range:     u8 n <= 6, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* offset is 1 – behaviour for n<1 is irrelevant */
        return 0;
    idx_t m = n - 1;                          /* m = n – 1 */
    seq_t prod = (seq_t)2 * (seq_t)m * (seq_t)m; /* 2·m² in seq_t space     */
    seq_t k = floor_sqrt(prod);                 /* k = ⌊√(2·m²)⌋ = ⌊m·√2⌋ */
    seq_t res = (seq_t)n + (k + 1) / 2;          /* a(n) = n + ⌊k+1⌋/2        */
    return res;
}
