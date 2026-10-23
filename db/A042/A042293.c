/*
 * A042293  Denominators of continued fraction convergents to sqrt(672).
 * https://oeis.org/A042293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 1. For n ≥ 2: a(n) = c[n] * a(n‑1) + a(n‑2),
 *            where the coefficient sequence c[n] repeats 1, 11, 1, 50
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned coeff[4] = {1, 11, 1, 50};

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t b_prev2 = (seq_t)1; /* a(0) */
    seq_t b_prev1 = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        unsigned c = coeff[(i - 1) & 3];   /* periodic coefficient 1,11,1,50 */
        seq_t b_curr = (seq_t)c * b_prev1 + b_prev2;
        b_prev2 = b_prev1;
        b_prev1 = b_curr;
    }
    return b_prev1;
}
