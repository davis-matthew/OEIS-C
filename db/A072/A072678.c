/*
 * A072678  Generalized Bell numbers B_{4,2}.
 * https://oeis.org/A072678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+3) = (8n^2+38n+51) * a(n+2) * - (2(n+1))(8n^3+32n^2+42n+13)
 *            * a(n+1) * - 8n(2n-1)(2n+1)(n+1)^2 * a(n) * Initial values:
 *            a(1)=1, a(2)=21, a(3)=1045 * The recurrence is valid for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and small indices directly */
    if (n == 0) return (seq_t)1;       /* a(0)=1, if desired */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)1045;

    /* Current window: a(i), a(i+1), a(i+2) */
    seq_t a_i     = (seq_t)1;      /* a(1) */
    seq_t a_i_1   = (seq_t)21;     /* a(2) */
    seq_t a_i_2   = (seq_t)1045;   /* a(3) */

    idx_t i; /* index of a_i, i starts at 1 for a(1) */
    for (i = 1; ; i++) {
        /* Compute a(i+3) using the recurrence */
        seq_t coeff1 = (seq_t)(8*i*i + 38*i + 51);
        seq_t term1  = coeff1 * a_i_2;

        /* coeff2 = 2*(i+1) * (8*i^3 + 32*i^2 + 42*i + 13) */
        seq_t coeff2_part = (seq_t)(2*(i+1));
        seq_t coeff2_poly = (seq_t)8 * (seq_t)i * (seq_t)i * (seq_t)i
                         + (seq_t)32 * (seq_t)i * (seq_t)i
                         + (seq_t)42 * (seq_t)i
                         + (seq_t)13;
        seq_t term2 = coeff2_part * coeff2_poly * a_i_1;

        /* coeff3 = 8*i*(2*i-1)*(2*i+1)*(i+1)^2 */
        seq_t coeff3 = (seq_t)8
                     * (seq_t)i
                     * (seq_t)(2*i-1)
                     * (seq_t)(2*i+1)
                     * (seq_t)(i+1)
                     * (seq_t)(i+1);
        seq_t term3 = coeff3 * a_i;

        /* a(i+3) = term1 - term2 - term3 */
        seq_t a_next = term1;
        a_next -= term2;
        a_next -= term3;

        /* Shift the window for next iteration */
        a_i = a_i_1;
        a_i_1 = a_i_2;
        a_i_2 = a_next;

        /* If we have reached the requested index, return it */
        if (i + 3 == n)
            return a_next;
    }
}
