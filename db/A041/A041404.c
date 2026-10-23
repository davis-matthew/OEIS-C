/*
 * A041404  Numerators of continued fraction convergents to sqrt(217).
 * https://oeis.org/A041404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(217).
   Recurrence: let a(-2)=0, a(-1)=1; for n≥0
   a(n) = c_n * a(n-1) + a(n-2),
   where c_n are the terms of the continued fraction
   expansion of sqrt(217): [14; 1,2,1,2,1,1,9,4,9,1,1,2,1,2,1,28] (period 16). */

seq_t a(idx_t n)
{
    /* Continued fraction coefficients after the initial term 14 */
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1,
        (seq_t)9, (seq_t)4, (seq_t)9, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)28
    };
    const seq_t a0 = (seq_t)14;
    const idx_t period_len = 16;

    seq_t p_nm2 = (seq_t)0;   /* a(-2) */
    seq_t p_nm1 = (seq_t)1;   /* a(-1) */
    seq_t p_n   = (seq_t)0;   /* a(n) */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        seq_t coeff = (k == (idx_t)0) ? a0
                    : period[(k - (idx_t)1) % period_len];
        p_n = coeff * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
