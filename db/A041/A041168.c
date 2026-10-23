/*
 * A041168  Numerators of continued fraction convergents to sqrt(94).
 * https://oeis.org/A041168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the n-th convergent of sqrt(94). 
   Recurrence: p(-2)=0, p(-1)=1, p(k)=a_k*p(k-1)+p(k-2) where
   a_0=9 and for k≥1, a_k is the (k-1)-st term of the periodic
   partial quotient list: {1,2,3,1,1,5,1,8,1,5,1,1,3,2,1,18}. */

seq_t a(idx_t n) {
    static const idx_t period[] = {
        1, 2, 3, 1, 1, 5, 1, 8,
        1, 5, 1, 1, 3, 2, 1, 18
    };
    static const idx_t period_len = sizeof(period) / sizeof(period[0]);

    /* The first convergent is trivial. */
    if (n == 0) return (seq_t)9;

    /* p(-2)=0, p(-1)=1 */
    seq_t p_nm2 = (seq_t)0;
    seq_t p_nm1 = (seq_t)1;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = (seq_t)9;
        } else {
            a_i = (seq_t)period[(i - 1) % period_len];
        }
        seq_t p_n = a_i * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_nm1;
}
