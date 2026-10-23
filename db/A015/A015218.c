/*
 * A015218  Inverse of 1209th cyclotomic polynomial.
 * https://oeis.org/A015218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence a(n) is the coefficient of x^n in the power series expansion
   of the reciprocal of the 1209th cyclotomic polynomial:
      1/Φ_1209(x) = (1 - x)(1 - x^39)(1 - x^93)(1 - x^403) /
                    ((1 - x^3)(1 - x^13)(1 - x^31)(1 - x^1209)).
   The sequence is periodic with period 1209.
*/

#include <stdint.h>

#define PERIOD 1209
#define MAX_D 47          /* largest sum of subset of {3,13,31} */
#define MAX_N 536         /* largest sum of subset of {1,39,93,403} */

seq_t a(idx_t n) {
    static seq_t cache[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        /* Coefficients of D(x) = (1 - x^3)(1 - x^13)(1 - x^31)(1 - x^1209) */
        int d[MAX_D + 1] = {0};
        d[0] = 1;
        const int d_vals[] = {3, 13, 31};
        for (int mask = 1; mask < (1 << 3); ++mask) {
            int sum = 0;
            int bits = 0;
            for (int i = 0; i < 3; ++i) {
                if (mask & (1 << i)) {
                    sum += d_vals[i];
                    ++bits;
                }
            }
            d[sum] = (bits % 2 == 0) ? 1 : -1;  /* sign = (-1)^bits */
        }

        /* Coefficients of N(x) = (1 - x)(1 - x^39)(1 - x^93)(1 - x^403) */
        int ncoef[MAX_N + 1] = {0};
        ncoef[0] = 1;
        const int n_vals[] = {1, 39, 93, 403};
        for (int mask = 1; mask < (1 << 4); ++mask) {
            int sum = 0;
            int bits = 0;
            for (int i = 0; i < 4; ++i) {
                if (mask & (1 << i)) {
                    sum += n_vals[i];
                    ++bits;
                }
            }
            ncoef[sum] = (bits % 2 == 0) ? 1 : -1;
        }

        /* Compute the first PERIOD coefficients via recurrence */
        cache[0] = 1;
        for (idx_t k = 1; k < PERIOD; ++k) {
            seq_t tmp = 0;
            idx_t max_i = (k < (idx_t)MAX_D) ? k : (idx_t)MAX_D;
            for (idx_t i = 1; i <= max_i; ++i) {
                if (d[i] != 0) {
                    tmp += (seq_t)d[i] * cache[k - i];
                }
            }
            seq_t n_val = (k <= (idx_t)MAX_N) ? (seq_t)ncoef[k] : (seq_t)0;
            cache[k] = -tmp + n_val;
        }
        initialized = 1;
    }
    return cache[n % PERIOD];
}
