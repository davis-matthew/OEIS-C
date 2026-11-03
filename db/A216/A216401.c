/*
 * A216401  E.g.f.: artanh(x*exp(x)).
 * https://oeis.org/A216401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor((n-1)/2)} (2*k+1)^(n-2*k-2) /
 *            (n-2*k-1)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 197 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    seq_t sum    = 0;
    seq_t product = (seq_t)n;                  /* n! / (n-1)!  */

    if (n & 1) {                              /* odd n */
        idx_t kmax = (n - 1) / 2;              /* exclusive upper bound */

        for (idx_t k = 0; k < kmax; ++k) {
            idx_t e = n - 2 * k - 2;           /* exponent (≥ 0) */
            seq_t term;

            if ((seq_t)(2 * k + 1) == (seq_t)1) {
                term = product;                /* 1^e = 1 */
            } else {
                seq_t pow = ipow((seq_t)(2 * k + 1),
                                    (seq_t)e);
                term = product * pow;
            }
            sum += term;

            if (k + 1 < kmax) {
                seq_t mult1 = (seq_t)(n - 2 * k - 2);
                seq_t mult2 = (seq_t)(n - 2 * k - 1);
                product = product * mult1 * mult2;
            }
        }

        /* last term = (n-1)! (k = (n-1)/2) */
        sum += factorial(n - 1);
    } else {                                   /* even n */
        idx_t half = n / 2;

        for (idx_t k = 0; k < half; ++k) {
            idx_t e = n - 2 * k - 2;
            seq_t term;

            if ((seq_t)(2 * k + 1) == (seq_t)1) {
                term = product;
            } else {
                seq_t pow = ipow((seq_t)(2 * k + 1),
                                    (seq_t)e);
                term = product * pow;
            }
            sum += term;

            if (k + 1 < half) {
                seq_t mult1 = (seq_t)(n - 2 * k - 2);
                seq_t mult2 = (seq_t)(n - 2 * k - 1);
                product = product * mult1 * mult2;
            }
        }
    }

    return sum;
}
