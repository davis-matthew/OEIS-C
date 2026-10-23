/*
 * A159233  Number of edge colorings of the Petersen graph.
 * https://oeis.org/A159233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 240 * Σ_{k=0}^{11, n ≥ k+4} q_k * C(n - k + 11, 15) where
 *            C(a,b) is the binomial coefficient, and q = [208, 75574,
 *            4153600, 68258235, 442507920, 1296079344, 1834236432,
 *            1285299570, 441713760, 71258450, 4939568, 120539] (coefficients
 *            from the GF)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 21, u128 n <= 372,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;            /* a(0) … a(3) = 0 */

    /* constants from the generating function */
    static const seq_t q[12] = {
        208ULL,      75574ULL,    4153600ULL,    68258235ULL,
        442507920ULL, 1296079344ULL, 1834236432ULL,
        1285299570ULL, 441713760ULL, 71258450ULL,
        4939568ULL,   120539ULL
    };

    seq_t sum = 0;

    for (idx_t k = 0; k < 12; ++k) {
        if (n < k + 4) continue;          /* term only contributes for n ≥ k+4 */

        idx_t m = n - k + 11;              /* binomial argument a = n‑k+11 */
        seq_t c = binom(m, 15);            /* C(m,15) */

        sum += c * q[k];
    }

    return sum * (seq_t)240;
}
