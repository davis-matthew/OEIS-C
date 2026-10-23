/*
 * A129184  Shift operator, right.
 * https://oeis.org/A129184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = 1 if n = k(k+3)/2 for some integer k ≥ 1, otherwise 0
 * Range:     u8 n <= 6, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          /* convert to seq_t */
    seq_t D = m * (seq_t)8 + (seq_t)9;   /* discriminant 9 + 8*n */

    seq_t r = floor_sqrt(D);           /* floor(sqrt(D)) */
    if (r * r != D) return 0;          /* D not a perfect square */

    if (r < (seq_t)3) return 0;         /* r-3 >= 0 required */

    seq_t numer = r - (seq_t)3;         /* r - 3 */
    if (numer & (seq_t)1) return 0;     /* numer must be even */

    seq_t k = numer >> 1;               /* (r-3)/2 */
    if (k < (seq_t)1) return 0;         /* k must be ≥ 1 */

    return 1;                           /* 1 found where n = k(k+3)/2 */
}
