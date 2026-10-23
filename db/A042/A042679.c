/*
 * A042679  Denominators of continued fraction convergents to sqrt(869).
 * https://oeis.org/A042679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = denominators of convergents to √869
 * Convergent recurrence: q(-1)=0, q(0)=1, q(n)=a(n)*q(n-1)+q(n-2) for n≥1
 * Partial quotients: a(0)=29, and for n≥1
 *   a(n) = { 2, 11, 3, 2, 1, 1, 1, 1, 1,
 *            14, 8, 2, 1, 4, 1, 2, 8, 14,
 *            1, 1, 1, 1, 1, 2, 3, 11, 2, 58 }
 * (period length 28)
 */

static const seq_t period_coeff[28] = {
    (seq_t)2,  (seq_t)11, (seq_t)3,  (seq_t)2,
    (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)1,
    (seq_t)1,  (seq_t)14, (seq_t)8,  (seq_t)2,
    (seq_t)1,  (seq_t)4,  (seq_t)1,  (seq_t)2,
    (seq_t)8,  (seq_t)14, (seq_t)1,  (seq_t)1,
    (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)2,
    (seq_t)3,  (seq_t)11, (seq_t)2,  (seq_t)58
};

seq_t a(idx_t n)
{
    seq_t q_minus2 = (seq_t)1; /* q(-1) = 0, but we keep shift in the loop */
    seq_t q_minus1 = (seq_t)0; /* q0 = 1 */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t coeff = (k == 0) ? (seq_t)29
                               : period_coeff[(k - 1) % 28];

        /* q(k) = coeff * q(k-1) + q(k-2) */
        seq_t q = coeff * q_minus1 + q_minus2;

        /* shift for next iteration */
        q_minus2 = q_minus1;
        q_minus1 = q;
    }
    return q_minus1;
}
