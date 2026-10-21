/*
 * A087809  Number of triangulations (by Euclidean triangles) having 3+3n
 *          vertices of a triangle with each side subdivided by n additional
 *          points.
 * https://oeis.org/A087809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1, if n == 0; otherwise a(n) = 2^(3*n-1) - 3 *
 *            Σ_{k=0}^{n-2} C(3*n-1, k) (OEIS A087809)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle offset */
    if (n == 0) return (seq_t)1;

    /* N = 3*n - 1 */
    idx_t N = 3 * n;
    --N;                       /* N = 3*n - 1 */

    /* Sum of binomial coefficients from k=0 to n-2 */
    seq_t sum = 0;
    seq_t c = 1;                /* C(N,0) */

    if (n > 1) {
        idx_t limit = n - 2;    /* last k to include */
        for (idx_t k = 0; k <= limit; ++k) {
            sum += c;
            if (k == limit) break;  /* avoid one extra computation */
            /* Update c to C(N, k+1) */
            seq_t num = (seq_t)(N - k);
            seq_t den = (seq_t)(k + 1);
            c = (c * num) / den;
        }
    }

    /* 2^(3*n-1) */
    seq_t pow_val = pow2(3 * n - 1);

    /* Result */
    seq_t result = pow_val - 3 * sum;
    return result;
}
