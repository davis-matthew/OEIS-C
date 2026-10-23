/*
 * A042090  Numerators of continued fraction convergents to sqrt(569).
 * https://oeis.org/A042090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(0) = 23 a(n) = c[n] * a(n-1) + a(n-2) for n > 0 where c[0] =
 *            23 and for n ≥ 1 the partial quotients repeat the period
 *            1,5,1,5,9,2,1,2,3,3,2,1,2,9,5,1,5,1,46
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned short a_cycle[19] = {
    1, 5, 1, 5, 9, 2, 1, 2, 3, 3, 2, 1, 2, 9, 5, 1, 5, 1, 46
};

seq_t a(idx_t n)
{
    /* Recurrence for numerators of convergents of sqrt(569). */
    seq_t p_minus2 = 0; /* a(-2) */
    seq_t p_minus1 = 1; /* a(-1) */
    seq_t p_curr   = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = 23;
        else
            coeff = a_cycle[(i - 1) % 19];

        p_curr   = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p_curr;
    }

    return p_curr;
}
