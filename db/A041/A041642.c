/*
 * A041642  Numerators of continued fraction convergents to sqrt(340).
 * https://oeis.org/A041642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of n-th convergent of sqrt(340)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrent: p[-2] = 0, p[-1] = 1, p[n] = a_n * p[n-1] + p[n-2] */
/* where a_0 = 18 and for n>0 : a_n = period[(n-1) % 14] */
/* period = {2,3,1,1,1,1,8,1,1,1,1,3,2,36} (the partial quotients of the continued fraction) */

seq_t a(idx_t n)
{
    /* Period of the continued fraction for sqrt(340) */
    static const seq_t PERIOD[14] = {
        2ULL, 3ULL, 1ULL, 1ULL, 1ULL, 1ULL,
        8ULL, 1ULL, 1ULL, 1ULL, 1ULL, 3ULL,
        2ULL, 36ULL
    };

    seq_t p_prev2 = 0;   /* p[-2] */
    seq_t p_prev1 = 1;   /* p[-1] */
    seq_t p = 0;         /* current numerator */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = 18ULL;                         /* a0 */
        else
            a_i = PERIOD[(i - 1) % 14U];         /* a_n for n>0 */

        p = a_i * p_prev1 + p_prev2;           /* recurrence */
        p_prev2 = p_prev1;                     /* advance sliding window */
        p_prev1 = p;
    }

    return p;
}
