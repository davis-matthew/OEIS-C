/*
 * A157778  Numerator of Euler(n, 15/32).
 * https://oeis.org/A157778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = Σ_{m=0}^{⌊n/2⌋} (-1)^n * C(n, 2m) * E_{2m} * 256^m where
 *            E_{2m} are the Euler (secant) numbers: E_0 = 1, E_2 = -1, E_4 =
 *            5, E_6 = -61, E_8 = 1385, … (OEIS A000364) The factor (-1)^n
 *            produces the two‑term sign alternation seen in the data
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---------- compute even Euler numbers up to index floor(n/2) ---------- */
    idx_t half = n / 2;
    seq_t *euler = (seq_t *)malloc((half + 1) * sizeof(seq_t));
    if (!euler)
        return 0;                         /* allocation failure: return 0 */

    euler[0] = 1;                          /* E_0 */

    for (idx_t m = 1; m <= half; ++m) {
        idx_t N = 2 * m;                    /* current even index */
        seq_t sum = 0;
        seq_t binom = 1;                    /* C(N,0) */

        for (idx_t k = 0; k < m; ++k) {
            /* term: C(N,2k) * E_{2k} */
            sum += binom * euler[k];

            /* update binomial coefficient to C(N, 2(k+1)) */
            seq_t num = binom;
            num *= (seq_t)(N - 2 * k);
            num *= (seq_t)(N - 2 * k - 1);
            num /= (seq_t)(2 * k + 2);
            num /= (seq_t)(2 * k + 1);
            binom = num;
        }

        euler[m] = -sum;                    /* E_{2m} = -Σ_{k=0}^{m-1} C(N,2k) E_{2k} */
    }

    /* ---------- compute a(n) using the derived formula ---------- */
    seq_t sum = 0;
    seq_t binom = 1;                        /* C(n,0) */
    seq_t pow256 = 1;                       /* 256^0 */

    for (idx_t m = 0; m <= half; ++m) {
        /* term: C(n,2m) * E_{2m} * 256^m */
        sum += binom * euler[m] * pow256;

        if (m == half)
            break;

        /* next power of 256 */
        pow256 *= 256;

        /* update binomial coefficient to C(n, 2(m+1)) */
        seq_t num = binom;
        num *= (seq_t)(n - 2 * m);
        num *= (seq_t)(n - 2 * m - 1);
        num /= (seq_t)(2 * m + 2);
        num /= (seq_t)(2 * m + 1);
        binom = num;
    }

    /* apply the factor (-1)^n */
    if (n & 1)
        sum = -sum;

    free(euler);
    return sum;
}
