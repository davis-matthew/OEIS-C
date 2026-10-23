/*
 * A042124  Numerators of continued fraction convergents to sqrt(587).
 * https://oeis.org/A042124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = numerator of convergents to sqrt(587). */
seq_t a(idx_t n)
{
    /* Continued fraction for sqrt(587): 24; (4, 2, 1, 1, 1, 1, 23, 1, 1, 1, 1, 2, 4, 48) repeating */
    const unsigned int period_len = 14;
    static const unsigned int period[14] = {
        4, 2, 1, 1, 1, 1, 23,
        1, 1, 1, 1, 2, 4, 48
    };

    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p_k;
    idx_t k;

    for (k = 0; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = (seq_t)24;
        else
            a_k = (seq_t)period[(k - 1) % period_len];

        p_k = a_k * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_k;
    }

    return p_nm1;
}
