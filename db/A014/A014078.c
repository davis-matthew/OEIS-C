/*
 * A014078  Inverse of 69th cyclotomic polynomial.
 * https://oeis.org/A014078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = -∑_{j=1}^{44} c_j a(n‑j),
   where c_j are the coefficients of the 69th cyclotomic polynomial
   Φ_69(x).  The sequence is periodic with period 69. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* coefficients of Φ_69(x) (c_0 = 1, c_j for j >= 1) */
    static const seq_t coeff[45] = {
        0,              /* unused c_0 */
        (seq_t)-1,      0,   1,  (seq_t)-1, 0,   1,  (seq_t)-1, 0,
          1, (seq_t)-1, 0,   1,  (seq_t)-1, 0,   1,  (seq_t)-1, 0,
          1, (seq_t)-1, 0,   1,  (seq_t)-1,          /* up to 22 */
          1, 0,          (seq_t)-1, 1, 0,          (seq_t)-1, 1,
          0,          (seq_t)-1, 1, 0,          (seq_t)-1, 1,
          0,          (seq_t)-1, 1, 0,          (seq_t)-1, 1,
          0,          (seq_t)-1, 1
    }; /* size 45, indices 0..44 */

    /* Periodic sequence of length 69 – computed once */
    static seq_t period[69];
    static int initialized = 0;

    if (!initialized) {
        /* first term */
        period[0] = (seq_t)1;

        /* compute terms 1..68 using the recurrence */
        for (idx_t i = 1; i < 69; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 44; ++j) {
                if (i >= j)
                    sum += coeff[j] * period[i - j];
            }
            period[i] = -sum;
        }
        initialized = 1;
    }

    return period[n % 69];
}
