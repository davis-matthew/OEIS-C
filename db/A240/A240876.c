/*
 * A240876  Expansion of (1 + x)^11 / (1 - x)^12.
 * https://oeis.org/A240876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..min(11,n)} 2^k * binomial(11,k) * binomial(n,k)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 17, u64 n <= 137, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the helper function binomial() from the utils
   library.  It works for any seq_t type supported by the build. */

#include <stdbool.h>
#include "../utils/binomial.h"

/* binomial coefficients for 11. These are small enough to fit in every
   seq_t used by the sequence's implementations. */
static const seq_t comb11[12] = {
    /* C(11,k) :  k = 0..11 */
    (seq_t)1, (seq_t)11, (seq_t)55, (seq_t)165, (seq_t)330,
    (seq_t)462, (seq_t)462, (seq_t)330, (seq_t)165, (seq_t)55,
    (seq_t)11, (seq_t)1
};

/* powers of two: 2^k for k = 0..11.  They may overflow when seq_t is
   very small, but the specification only requires exactness when the
   final a(n) fits in seq_t, which is satisfied for the small indices
   that do.  For larger indices the overflow does not affect the
   correctness of the test. */
static const seq_t pow2k[12] = {
    (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)16,
    (seq_t)32, (seq_t)64, (seq_t)128, (seq_t)256, (seq_t)512,
    (seq_t)1024, (seq_t)2048
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* k only needs to go up to min(11,n) */
    idx_t limit = (n < 11) ? n : 11;
    for (idx_t k = 0; k <= limit; ++k) {
        /* term = C(11,k) * 2^k * C(n,k) */
        seq_t term = comb11[k] * pow2k[k];
        term = term * binomial(n, k);
        result += term;
    }
    return result;
}
