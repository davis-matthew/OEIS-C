/*
 * A015648  Inverse of 1639th cyclotomic polynomial.
 * https://oeis.org/A015648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = coefficient of x^n in the reciprocal power series of the
 *            1639th cyclotomic polynomial. The sequence is periodic with
 *            period 1639
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static seq_t phi[1481];          /* coefficients of Φ_1639(x)   */
static seq_t a_cache[1639];      /* a(n) for n = 0..1638       */
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Compute Φ_1639(x) via division :
           N(x) = (x^1640 - x^1639 - x + 1)
           D(x) = (x^160 - x^149 - x^11 + 1)
           Φ(x) = N(x)/D(x)  (deg 1480) */
        const int deg_phi = 1480;
        const int deg_den = 160;

        for (int i = 0; i <= deg_phi; ++i) {
            seq_t n_i = 0;
            if (i == 0) n_i = 1;       /* from N(x) */
            else if (i == 1) n_i = -1; /* from N(x) */
            /* other n_i are 0 */

            seq_t val = n_i;
            if (i - 11 >= 0)   val += phi[i - 11];
            if (i - 149 >= 0)  val += phi[i - 149];
            if (i - 160 >= 0)  val -= phi[i - 160];
            phi[i] = val;
        }

        /* Compute inverse series a(n) such that
           Φ(x) * a(x) = 1 */
        const int period = 1639;
        a_cache[0] = 1;
        for (int m = 1; m < period; ++m) {
            seq_t sum = 0;
            int kmax = (m < deg_phi) ? m : deg_phi;
            for (int k = 1; k <= kmax; ++k) {
                sum += phi[k] * a_cache[m - k];
            }
            a_cache[m] = -sum;
        }
        initialized = true;
    }

    /* Return value with periodicity */
    size_t idx = (size_t)(n % 1639);
    return a_cache[idx];
}
