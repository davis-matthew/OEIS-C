/*
 * A042898  Numerators of continued fraction convergents to sqrt(981).
 * https://oeis.org/A042898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of √981
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 16, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic partial quotients of √981: a0 … a30 */
    static const seq_t coeffs[31] = {
        31, 3, 8, 1, 1, 1, 1, 1, 1, 2,
        1, 1, 15, 12, 2, 6, 2, 12, 15,
        1, 1, 2, 1, 1, 1, 1, 1, 1, 8,
        3, 62
    };

    seq_t p_prev_prev = 0;  /* p{-2} */
    seq_t p_prev      = 1;  /* p{-1} */
    seq_t p_curr      = 0;  /* current numerator */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t a_k;
        if (k <= 30)
            a_k = coeffs[k];
        else
            a_k = coeffs[1 + ((k - 1) % 30)]; /* repeat the period */

        p_curr = a_k * p_prev + p_prev_prev;

        /* shift for next iteration */
        p_prev_prev = p_prev;
        p_prev = p_curr;
    }

    return p_curr;
}
