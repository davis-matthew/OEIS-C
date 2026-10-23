/*
 * A011829  Number of f-vectors for simplicial complexes of dimension at most
 *          4 on at most n-1 vertices.
 * https://oeis.org/A011829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{16} q_k * binomial(n−k+15, 15) where q = [2,
 *            −29, 197, −830, 2426, −5196, 8548, −9460, 17282, 6585, 16919,
 *            2541, 1154, −114, 16, −1]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdlib.h>  /* abs() */

/* coefficients q_k for k = 1..16 */
static const int q[17] = {
    0,          /* unused, q[0] */
    2,          /* q1 */
   -29,         /* q2 */
    197,        /* q3 */
   -830,        /* q4 */
   2426,        /* q5 */
  -5196,        /* q6 */
   8548,        /* q7 */
  -9460,        /* q8 */
  17282,        /* q9 */
   6585,        /* q10 */
  16919,        /* q11 */
   2541,        /* q12 */
   1154,        /* q13 */
   -114,        /* q14 */
    16,         /* q15 */
   -1          /* q16 */
};

/* Count of f‑vectors on ≤n−1 vertices, dimension ≤4 */
seq_t a(idx_t n)
{
    seq_t pos = 0;
    seq_t neg = 0;
    for (int k = 1; k <= 16; ++k) {
        idx_t t = n + 15 - k;          /* n − k + 15   */
        if (t < 15) break;             /* further terms are zero */
        seq_t bin = binomial(t, 15);   /* C(t,15)      */
        seq_t term = bin * (seq_t)abs(q[k]);  /* |q_k| * bin  */
        if (q[k] >= 0)
            pos += term;
        else
            neg += term;
    }
    /* pos ≥ neg by construction of the sequence */
    return pos - neg;
}
