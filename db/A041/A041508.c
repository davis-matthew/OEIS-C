/*
 * A041508  Numerators of continued fraction convergents to sqrt(271).
 * https://oeis.org/A041508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(271):
   p_{-1} = 1, p_0 = 16
   for n >= 1:  p_n = a_n * p_{n-1} + p_{n-2}
   where the continued fraction coefficients a_n (for n >= 1)
   repeat with period 24:
   [2,6,10,1,4,1,1,2,1,2,1,15,1,2,1,2,1,1,4,1,10,6,2,32] */
seq_t a(idx_t n)
{
    /* periodic continued fraction coefficients for sqrt(271) */
    static const seq_t cf[24] = {2,6,10,1,4,1,1,2,1,2,1,15,1,2,1,2,1,1,4,1,10,6,2,32};

    if (n == 0) return 16;          /* a(0) */
    seq_t p_prev2 = 1;              /* p_{-1} */
    seq_t p_prev1 = 16;             /* p_0 */
    seq_t cur;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = cf[(i-1) % 24]; /* a_i for i>=1 */
        cur = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = cur;
    }
    return p_prev1;
}
