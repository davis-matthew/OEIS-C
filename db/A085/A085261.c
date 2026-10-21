/*
 * A085261  Expansion of chi(x) / phi(x^2) in powers of x where phi(), chi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A085261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of x^n in * ∏_{k≥1} (1 + x^(2k-1)) / ((1
 *            - x^(4k)) * (1 + x^(4k-2))^2). * This is computed by multiplying
 *            the polynomial by each factor * (1 - x^m)^e_m where the exponent
 *            e_m is * e_m = -1 if m is odd * e_m = 3 if m ≡ 2 (mod 4) * e_m =
 *            -3 if m ≡ 0 (mod 4) and (m/4) odd * e...
 * Range:     i8 n <= 15, i16 n <= 47, i32 n <= 145, i64 n <= 493,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle offset 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* allocate array of coefficients a[0..n] */
    size_t N = (size_t)n;
    seq_t *coef = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!coef) {
        return (seq_t)0;            /* unlikely */
    }
    coef[0] = (seq_t)1;

    /* iterate over all m = 1 .. N */
    for (size_t m = 1; m <= N; ++m) {
        int e;
        if (m & 1) {                /* odd */
            e = -1;
        } else if ((m & 3) == 2) {  /* m ≡ 2 mod 4 */
            e = 3;
        } else {                    /* m ≡ 0 mod 4 */
            size_t k = m >> 2;      /* k = m/4 */
            e = (k & 1) ? -3 : -1;
        }

        if (e > 0) {                /* multiply by (1 - x^m)^e  -> subtract e times */
            for (int r = 0; r < e; ++r) {
                for (size_t i = N; i >= m; --i) {
                    coef[i] -= coef[i - m];
                }
            }
        } else if (e < 0) {         /* multiply by (1 - x^m)^e  -> add |e| times */
            int abs_e = -e;
            for (int r = 0; r < abs_e; ++r) {
                for (size_t i = m; i <= N; ++i) {
                    coef[i] += coef[i - m];
                }
            }
        }
    }

    seq_t res = coef[N];
    free(coef);
    return res;
}
