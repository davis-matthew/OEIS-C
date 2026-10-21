/*
 * A079280  Number of log-concave paths of length n starting from the origin
 *          (0,0) with steps from {N=(0,1), E=(1,0) and S=(0,-1)} that stay in
 *          the second octant and never touch the line y=x except possibly at
 *          the beginning or the end.
 * https://oeis.org/A079280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = (4^(n-1) + 3*C(2*n-2, n-1)) / 2 Formula: a(2*n+1) =
 *            (2*4^(n-1) + C(2*n, n)) / 2 (a(1) = 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial_mul.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* harmless for calls < offset */
    if (n == 1) return (seq_t)1;         /* a(1) = 1 */

    if ((n & 1) == 0) {                  /* even index: n = 2*m */
        idx_t m = n >> 1;                /* m = n/2 */

        /* compute 4^(m-1) */
        seq_t pow4 = (seq_t)1;
        for (idx_t i = (idx_t)1; i < m; ++i)
            pow4 *= (seq_t)4;

        seq_t bin = binomial_mul((idx_t)(2 * m - 2), (idx_t)(m - 1));
        return (pow4 + (seq_t)3 * bin) >> 1;   /* divide by 2 */
    } else {                              /* odd index: n = 2*m + 1 */
        idx_t m = (n - 1) >> 1;           /* m = (n-1)/2 */

        /* compute 4^(m-1) */
        seq_t pow4 = (seq_t)1;
        for (idx_t i = (idx_t)1; i < m; ++i)
            pow4 *= (seq_t)4;

        seq_t bin = binomial_mul((idx_t)(2 * m), (idx_t)m);
        return ((seq_t)2 * pow4 + bin) >> 1; /* divide by 2 */
    }
}
