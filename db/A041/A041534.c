/*
 * A041534  Numerators of continued fraction convergents to sqrt(284).
 * https://oeis.org/A041534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the convergents to sqrt(284).  
   a(0) = 16, a(n) = a_n * a(n-1) + a(n-2) for n≥1,
   where a_n are the continued‑fraction coefficients of sqrt(284):
   {1,5,1,3,2,1,4,8,4,1,2,3,1,5,1,32} repeat cyclically. */

seq_t a(idx_t n) {
    /* Periodic continued‑fraction coefficients for sqrt(284). */
    static const seq_t cf[16] = {
        (seq_t)1, 5, 1, 3, 2, 1,
        4, 8, 4, 1, 2, 3,
        1, 5, 1, 32
    };

    /* Base cases handled by the recurrence with p[-2]=0, p[-1]=1. */
    seq_t p_nm2 = (seq_t)0;          /* p_{-2} */
    seq_t p_nm1 = (seq_t)1;          /* p_{-1} */
    seq_t p_n   = 0;                 /* current numerator */

    /* Compute up to the requested index n. */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)16;                /* a_0 = floor(sqrt(284)) */
        else
            a_i = cf[(i - 1) % 16];          /* periodic pattern */

        p_n = a_i * p_nm1 + p_nm2;

        /* Shift for next iteration. */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }

    return p_n;
}
