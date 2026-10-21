/*
 * A208187  Number of distinct n-colored necklaces with 6 beads per color.
 * https://oeis.org/A208187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|6} phi(6/d) * (n*d)! / (d!^n * n * 6), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n>0 the sum is first computed in the numerator and
   finally divided by n*6. This guarantees exact integer division. */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Divisors of 6 in increasing order */
    const idx_t ds[4] = {1, 2, 3, 6};
    /* Pre‑computed φ(6/d) values for these divisors:
       d=1 → φ(6)=2, d=2 → φ(3)=2, d=3 → φ(2)=1, d=6 → φ(1)=1 */
    const seq_t phis[4] = {(seq_t)2, (seq_t)2, (seq_t)1, (seq_t)1};

    seq_t numerator = 0;

    for (size_t k = 0; k < 4; ++k) {
        idx_t d = ds[k];
        idx_t total = n * d;          /* total = n*d                                    */
        seq_t mult = (seq_t)1;        /* mult = (n*d)! / (d!^n)                          */

        /* Compute mult multiplicatively: Π_{i=0}^{n-1} binomial(total - i*d, d)  */
        for (idx_t i = 0; i < n; ++i) {
            mult *= binomial(total - i * d, d);
        }

        seq_t term_num = phis[k] * mult; /* φ(6/d) * mult                                 */
        numerator += term_num;
    }

    seq_t denom = (seq_t)n * 6;
    return numerator / denom;
}
