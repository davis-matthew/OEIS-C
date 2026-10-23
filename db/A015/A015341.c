/*
 * A015341  Gaussian binomial coefficient [ n,7 ] for q = -4.
 * https://oeis.org/A015341
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,7] for q = -4 * (offset
 *            = 7; a(7) is the first term). * Computation is done using the
 *            standard recurrence for fixed k=7: * G(n,k) = G(n-1,k) + q^(n-k)
 *            * G(n-1,k-1), q = -4 * Starting with G(0,0)=1 and G(0,j>0)=0.
 *            The recurrence is evaluated * iteratively up...
 * Range:     i8 n <= 7, i16 n <= 8, i32 n <= 9, i64 n <= 11, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)7)
        return (seq_t)0;   /* undefined for n < 7 */

    /* g[j] will hold G(current n, j) for j=0..7 */
    seq_t g[8] = {0};
    g[0] = (seq_t)1;               /* G(n,0)=1 for all n */

    /* pow[j] holds q^(current n - j), updated each iteration */
    seq_t pow[8] = {0};

    for (idx_t cur = 1; cur <= n; ++cur) {
        int maxj = cur < 7 ? (int)cur : 7;

        /* Update powers: for j == cur            -> exponent 0 => 1
         *                for j < cur             -> multiply previous by q
         * Powers for j > cur are irrelevant */
        for (int j = maxj; j >= 1; --j) {
            if (j == (int)cur)
                pow[j] = (seq_t)1;
            else
                pow[j] *= (seq_t)-4;   /* q = -4 */
        }

        /* Apply recurrence descending to avoid overwriting needed values */
        for (int j = maxj; j >= 1; --j) {
            g[j] += pow[j] * g[j - 1];
        }
    }

    return g[7];
}
