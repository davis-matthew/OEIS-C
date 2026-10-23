/*
 * A009121  Expansion of e.g.f. cosh(exp(x)*x).
 * https://oeis.org/A009121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A009121
    Formula: a(n) = Σ_{m=0}^{⌊n/2⌋} binomial(n, 2m) * (2m)^{\,n-2m}
    (exponential generating function: cosh(exp(x) * x))
*/

#include <stdint.h>
#include "../utils/pow_seq.h"  /* pow_seq(seq_t a, int k) */
#include "../utils/binomial.h" /* binomial(idx_t n, idx_t k) */

typedef uint64_t idx_t;  /* defined by build system – included here for clarity */

/* compute a(n) using an efficient recurrence for binomial(n,2m) */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t binom   = 1;          /* binomial(n, 0) */
    idx_t half_n  = n / 2;

    for (idx_t m = 0; m <= half_n; ++m) {
        idx_t two_m = 2 * m;
        idx_t expo  = n - two_m;

        /* compute (2m)^{expo}   (handle 0^0 to be 1) */
        seq_t pow_val;
        if (two_m == 0) {
            pow_val = (expo == 0) ? 1 : 0;
        } else {
            pow_val = pow_seq((seq_t)two_m, (int)expo);
        }

        result += binom * pow_val;

        /* update binom for next even (2m+2) */
        if (m < half_n) {
            seq_t top    = (seq_t)(n - two_m) * (seq_t)(n - two_m - 1);
            seq_t bottom = (seq_t)(two_m + 1) * (seq_t)(two_m + 2);
            binom = (binom * top) / bottom;
        }
    }

    return result;
}
