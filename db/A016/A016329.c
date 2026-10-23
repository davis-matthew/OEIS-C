/*
 * A016329  126th cyclotomic polynomial.
 * https://oeis.org/A016329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 36, i16 n <= 36, i32 n <= 36, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coefficients of the cyclotomic polynomial Φ_126(x):
   a(n) = coefficient of x^n in Φ_126(x).  The polynomial has degree
   φ(126) = 36, so a(n)=0 for n>36. */
#include <stddef.h>

#define DEG 36

seq_t a(idx_t n)
{
    static seq_t coeff[DEG + 1];
    static int initialized = 0;

    if (!initialized) {
        /* start from the constant polynomial 1 */
        for (int i = 0; i <= DEG; ++i)
            coeff[i] = 0;
        coeff[0] = 1;

        /* list of divisors d of 126 with μ(126/d) ≠ 0 and d ≤ DEG.
           The Möbius values:  μ(126/d) = -1 for d=3,18; = +1 for d=6,9,21. */
        static const struct {
            int d;
            int mu;
        } steps[] = {
            {3,  -1},   /* μ(42) = -1 */
            {6,   1},   /* μ(21) = +1 */
            {9,   1},   /* μ(14) = +1 */
            {18, -1},   /* μ(7)  = -1 */
            {21,  1}    /* μ(6)  = +1 */
        };
        const size_t nsteps = sizeof(steps) / sizeof(steps[0]);

        for (size_t idx = 0; idx < nsteps; ++idx) {
            int d  = steps[idx].d;
            int mu = steps[idx].mu;

            if (mu == 1) { /* multiply by (1 - x^d) */
                for (int i = DEG; i >= d; --i)
                    coeff[i] = coeff[i] - coeff[i - d];
            } else { /* mu == -1: multiply by 1/(1 - x^d) = 1 + x^d + x^{2d} + … */
                seq_t newcoeff[DEG + 1];
                for (int i = 0; i <= DEG; ++i) {
                    seq_t val = coeff[i];
                    if (i >= d)
                        val = val + newcoeff[i - d];
                    newcoeff[i] = val;
                }
                for (int i = 0; i <= DEG; ++i)
                    coeff[i] = newcoeff[i];
            }
        }
        initialized = 1;
    }

    return (n <= DEG) ? coeff[(int)n] : 0;
}
