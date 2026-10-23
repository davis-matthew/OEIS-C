/*
 * A041636  Numerators of continued fraction convergents to sqrt(337).
 * https://oeis.org/A041636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=18, a(1)=37; for n>=2
   a(n) = c[n]*a(n-1) + a(n-2),
   where c[n] are the continued‑fraction coefficients of √337:
   a0 = 18;
   a_{k} = 2,1,3,1,11,2,4,1,3,3,1,4,2,11,1,3,1,2,36 (period 19). */

static const seq_t cf_period[19] = {
    2, 1, 3, 1, 11, 2, 4, 1, 3, 3,
    1, 4, 2, 11, 1, 3, 1, 2, 36
};

seq_t a(idx_t n)
{
    seq_t p_prev2 = 0;   /* P_{-2} */
    seq_t p_prev1 = 1;   /* P_{-1} */
    seq_t p = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)18;
        else
            coeff = cf_period[(i - 1) % 19u];

        p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p;
}
