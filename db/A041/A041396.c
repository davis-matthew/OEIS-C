/*
 * A041396  Numerators of continued fraction convergents to sqrt(213).
 * https://oeis.org/A041396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(213).  
   Recurrence: a(0)=14, a(1)=15; for n≥2  
   a(n)=c_n*a(n-1)+a(n-2) where c_n are the continued‑fraction coefficients  
   of sqrt(213): [1,1,2,6,1,8,1,6,2,1,1,28] repeating after the first term. */

seq_t a(idx_t n)
{
    /* Periodic coefficients of sqrt(213), starting from c_1 (i=1) */
    static const seq_t period[12] = {
        (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)6,
        (seq_t)1, (seq_t)8, (seq_t)1, (seq_t)6,
        (seq_t)2, (seq_t)1, (seq_t)1, (seq_t)28
    };
    const idx_t PERIOD_LEN = 12;

    /* Convergent recurrence */
    seq_t p_im2 = (seq_t)0;   /* p_{-2} */
    seq_t p_im1 = (seq_t)1;   /* p_{-1} */
    seq_t p = (seq_t)0;       /* current numerator */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = (seq_t)14;
        else
            ai = period[(i - 1) % PERIOD_LEN];

        p = ai * p_im1 + p_im2;
        p_im2 = p_im1;
        p_im1 = p;
    }
    return p;
}
