/*
 * A041531  Denominators of continued fraction convergents to sqrt(282).
 * https://oeis.org/A041531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to √282. The
 *            convergents q_n satisfy q_n = a_n * q_{n-1} + q_{n-2} where the
 *            continued‑fraction coefficients for √282 are [16;
 *            1,3,1,4,1,3,1,32] repeating for n ≥ 1. Initial values: q_{-1}=0,
 *            q_0=1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q_prev2 = (seq_t)0;      /* q_{-1} */
    seq_t q_prev1 = (seq_t)1;      /* q_0 = 1  */
    if (n == (idx_t)0)
        return q_prev1;

    for (idx_t i = 1; i <= n; ++i) {
        unsigned cf;
        idx_t r = (i - 1) % 8ULL;  /* period index */
        switch (r) {
            case 0: cf = 1;  break;
            case 1: cf = 3;  break;
            case 2: cf = 1;  break;
            case 3: cf = 4;  break;
            case 4: cf = 1;  break;
            case 5: cf = 3;  break;
            case 6: cf = 1;  break;
            case 7: cf = 32; break;
        }
        seq_t q_curr = ((seq_t)cf) * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }
    return q_prev1;
}
