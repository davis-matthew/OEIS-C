/*
 * A131324  Triangle read by rows: T(n,k) = 2*|A049310(n,k)| - (-1)^(n+k).
 * https://oeis.org/A131324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened triangle entry T(r,k) where r =
 *            ⌊(√(8n+1)−1)/2⌋ (row index) and k = n − r(r+1)/2 (column index).
 *            T(r,k) = 1 if (r+k) is odd, 2·binomial((r+k)/2, k) – 1 if (r+k)
 *            is even
 * Range:     u8 n <= 7, u16 n <= 360, u32 n <= 1096, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/binom.h"        /* binom(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Compute r = floor((sqrt(8*n+1)-1)/2) */
    seq_t tmp = (seq_t)n * 8 + 1;          /* 8*n + 1, as seq_t */
    seq_t s   = floor_sqrt(tmp);          /* floor sqrt */
    idx_t r   = (idx_t)((s - 1) / 2);     /* row index */

    /* start index of row r */
    seq_t row_start = (seq_t)r * (r + 1) / 2;
    idx_t k         = n - (idx_t)row_start;   /* column index */

    /* Determine if (r + k) is even */
    if ( ((r + k) & 1) != 0 )
        return (seq_t)1; /* odd sum -> T = 1 */

    /* Even sum: compute T = 2*binomial((r+k)/2, k) - 1 */
    idx_t m = (r + k) / 2;                 /* (r+k)/2 */
    seq_t bin = binom(m, k);               /* binomial(m, k) */
    return (bin << 1) - 1;                /* 2*bin - 1 */
}
