/*
 * A208275  The number of partitions of the set [n] where each element can be
 *          colored 1 or 2 avoiding the patterns 1^11^1 and 1^22^1 in the
 *          pattern sense.
 * https://oeis.org/A208275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived formula:
   a(n) = ∑_{j=0}^{⌊(n-1)/2⌋} binom(n,2j+1)·j!
          + ∑_{j=0}^{⌊(n-2)/2⌋} (j+1)·j!·binom(n,2j+2)
          + ∑_{j=0}^{⌊(n-2)/2⌋} binom(n-1,2j+1)·j! + 1
   The implementation evaluates these sums in O(n) time by iterating
   over j and updating the binomial coefficients multiplicatively.
*/

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 // offset 1; behaviour below is irrelevant
    const seq_t xn = (seq_t)n;             // cast once for speed

    int jmax1 = (int)((n - 1) / 2);        // upper limit for the first sum
    int jmax2 = (int)((n - 2) / 2);        // upper limit for the second and third sums

    /* factorial j! (starting at 0! = 1) */
    seq_t fact = 1;

    /* binomial coefficients for the different sequences */
    seq_t odd      = xn;                                  // C(n,1)
    seq_t even     = (n >= 2) ? ((seq_t)n * (xn - 1)) / 2 : 0;   // C(n,2)
    seq_t odd_nm1  = xn - 1;                               // C(n-1,1)

    seq_t result = 0;

    for (int j = 0; j <= jmax1; ++j) {
        /* first sum term */
        result += odd * fact;

        if (j <= jmax2) {
            /* second sum term */
            seq_t tmp = (seq_t)(j + 1) * fact;
            result += tmp * even;

            /* third sum term */
            result += odd_nm1 * fact;
        }

        /* update factorial for next j */
        fact *= (seq_t)(j + 1);

        /* advance binomial coefficients for next j */
        if (j < jmax1) {
            /* update odd: C(n, 2j+3) from C(n, 2j+1) */
            seq_t k = (seq_t)(2 * j + 1);
            seq_t num = (xn - k) * (xn - k - 1);
            seq_t den = (k + 1) * (k + 2);
            odd = odd * num / den;
        }
        if (j < jmax2) {
            /* update even: C(n, 2j+4) from C(n, 2j+2) */
            seq_t k = (seq_t)(2 * j + 2);
            seq_t num = (xn - k) * (xn - k - 1);
            seq_t den = (k + 1) * (k + 2);
            even = even * num / den;

            /* update odd_nm1: C(n-1, 2j+3) from C(n-1, 2j+1) */
            seq_t k_nm1 = (seq_t)(2 * j + 1);
            seq_t num_nm1 = (xn - 1 - k_nm1) * (xn - 1 - k_nm1 - 1);
            seq_t den_nm1 = (k_nm1 + 1) * (k_nm1 + 2);
            odd_nm1 = odd_nm1 * num_nm1 / den_nm1;
        }
    }

    /* final "+1" term */
    result += 1;
    return result;
}
