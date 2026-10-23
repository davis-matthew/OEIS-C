/*
 * A193514  Expansion of phi(-q)^2 * phi(-q^9) / phi(-q^3) in powers of q
 *          where phi() is a Ramanujan theta function.
 * https://oeis.org/A193514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in φ(-q)^2 * φ(-q^9) / φ(-q^3)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where φ(q) = ∑_{k∈ℤ} q^{k^2} is the Ramanujan theta function. */

#include <stdlib.h>
#include <stdint.h>

static const int binom[5][5] = {
    {1,0,0,0,0},
    {1,1,0,0,0},
    {1,2,1,0,0},
    {1,3,3,1,0},
    {1,4,6,4,1}
};

seq_t a(idx_t n)
{
    static seq_t *series = NULL;
    static size_t cap = 0;

    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(series, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        series = tmp;

        /* recompute from scratch up to new_cap-1 */
        for (size_t i = 0; i < new_cap; ++i) series[i] = 0;
        series[0] = 1;

        for (int m = 1; m <= (int)n; ++m) {
            /* compute exponent e_m in the factor (1 - q^m)^{e_m} */
            int e = 0;
            if (m % 2 == 0) e += 2;        /* even m: +2 from φ(-q)^2 */
            else            e += 4;        /* odd m:  +4 from φ(-q)^2 */

            if (m % 18 == 0) e += 1;      /* φ(-q^9): even part */
            if (m % 18 == 9) e += 2;      /* φ(-q^9): odd part  */

            if (m % 6 == 0)  e -= 1;      /* divide by φ(-q^3): even part */
            if (m % 6 == 3)  e -= 2;      /* divide by φ(-q^3): odd part   */

            if (e == 0) continue;         /* skip trivial factor */

            /* coefficients of (1 - q^m)^e */
            seq_t coeff[5];
            coeff[0] = 1;
            for (int j = 1; j <= e; ++j) {
                coeff[j] = (seq_t)((j & 1) ? -binom[e][j] : binom[e][j]);
            }

            /* in-place DP: descending indices to avoid using updated values */
            for (size_t i = new_cap; i-- > (size_t)m; ) {
                seq_t val = series[i];
                for (int j = 1; j <= e; ++j) {
                    size_t mj = (size_t)j * (size_t)m;
                    if (i < mj) break;          /* would underflow */
                    val += coeff[j] * series[i - mj];
                }
                series[i] = val;
            }
        }
        cap = new_cap;
    }
    return series[(size_t)n];
}
