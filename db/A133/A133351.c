/*
 * A133351  Dimensions of certain Lie algebra (see reference for precise
 *          definition).
 * https://oeis.org/A133351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n,9)} q_k * C(n-k+12,12) where q = [1,
 *            176, 4341, 30221, 76141, 76141, 30221, 4341, 176, 1]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 74, u128 n <= 3080,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficient function binom(idx_t a, idx_t b) is provided in
   the utilities; it returns a seq_t value equal to (a choose b). */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Coefficients q_k of Q(x) = (x+1)*x^8 + ... +1 */
    static const idx_t q[10] = {
        1ULL,        /* q0 */
        176ULL,      /* q1 */
        4341ULL,     /* q2 */
        30221ULL,    /* q3 */
        76141ULL,    /* q4 */
        76141ULL,    /* q5 */
        30221ULL,    /* q6 */
        4341ULL,     /* q7 */
        176ULL,      /* q8 */
        1ULL         /* q9 */
    };

    seq_t result = 0;
    /* Only terms with k <= n contribute */
    idx_t limit = n;
    if (limit > 9) limit = 9;

    for (idx_t k = 0; k <= limit; ++k)
    {
        /* Compute binomial coefficient C(n - k + 12, 12) */
        seq_t bin = binom(n - k + 12, 12);
        /* Add q[k] * bin to result */
        result += (seq_t)q[k] * bin;
    }

    return result;
}
