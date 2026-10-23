/*
 * A036772  Number of labeled rooted trees with a degree constraint:
 *          ((4*n)!/(24^n)) * binomial(4*n+1, n).
 * https://oeis.org/A036772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4*n)!/(24^n)) * binomial(4*n+1, n) Recurrence: a(0) = 1
 *            a(n+1) = a(n) * ( 8*(4n+1)*(4n+3)^2*(2n+1)^2*(4n+5) ) / ( 81n^2
 *            + 162n + 72 )
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the recurrence above.  All intermediate
   values are kept in the type seq_t (which may be 8, 16, 32, 64,
   128‑bit or an arbitrary‑precision type).  GCD reductions are
   performed to avoid overflow on the 64‑bit build, while the
   arbitrary‑precision build uses the built‑in large‑integer
   arithmetic provided by the build environment. */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t res = 1;             /* a(0) */

    for (idx_t i = 0; i < n; ++i) {
        /* Build numerator: 8*(4i+1)*(4i+3)^2*(2i+1)^2*(4i+5) */
        seq_t num = 8;
        num *= (seq_t)(4ULL * i + 1ULL);

        seq_t t3 = (seq_t)(4ULL * i + 3ULL);
        num *= t3 * t3;              /* (4i+3)^2 */

        seq_t t2 = (seq_t)(2ULL * i + 1ULL);
        num *= t2 * t2;              /* (2i+1)^2 */

        num *= (seq_t)(4ULL * i + 5ULL);

        /* Denominator: 81*i^2 + 162*i + 72 */
        seq_t den = (seq_t)81 * (seq_t)i * (seq_t)i
                    + (seq_t)162 * (seq_t)i + 72;

        /* Cancel common factors between current result and denominator */
        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        /* Cancel remaining common factors between numerator and denominator */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* At this point den should be 1 (by the integer property of the recurrence) */
        res = res * num;
        if (den != 1) {
            res /= den;
        }
    }

    return res;
}
