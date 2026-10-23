/*
 * A042440  Numerators of continued fraction convergents to sqrt(748).
 * https://oeis.org/A042440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 14, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(748). */
/* The recurrence is: p_{-2} = 0, p_{-1} = 1, p_n = a_n * p_{n-1} + p_{n-2} */
/* where a_0 = 27 and the following periodic part a_1..a_12 = {2,1,6,5,1,12,1,5,6,1,2,54} */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction for sqrt(748) */
    static const unsigned int cf_pattern[12] = {2,1,6,5,1,12,1,5,6,1,2,54};

    seq_t p_prev2 = 0; /* p_{-2} */
    seq_t p_prev1 = 1; /* p_{-1} */
    seq_t p = 0;       /* current p_n */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned int a_k = (k == 0) ? 27
                                   : cf_pattern[(k - 1) % 12];
        p = (seq_t)a_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
