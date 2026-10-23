/*
 * A042318  Numerators of continued fraction convergents to sqrt(686).
 * https://oeis.org/A042318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(686)
   a(0) = 26
   a(n) = a_{n-1} * coeff_n + a_{n-2}
   where coeff_0 = 26, coeff_1..coeff_16 = 5,4,1,1,3,2,10,26,10,2,3,1,1,4,5,52
   and coeff_{k+16} = coeff_k (periodic with period 16) */
seq_t a(idx_t n)
{
    /* partial quotients of the continued fraction of sqrt(686) after a0=26 */
    static const seq_t period[16] = {5, 4, 1, 1, 3, 2, 10, 26,
                                     10, 2, 3, 1, 1, 4, 5, 52};

    /* base cases handled by the loop below */
    seq_t h_minus2 = (seq_t)0;   /* h_{-2} */
    seq_t h_minus1 = (seq_t)1;   /* h_{-1} */
    seq_t h_curr  = (seq_t)0;    /* h_n */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff;
        if (i == 0)
            coeff = (seq_t)26;       /* a0 */
        else
            coeff = period[(i - 1) % 16];

        h_curr = coeff * h_minus1 + h_minus2;
        h_minus2 = h_minus1;
        h_minus1 = h_curr;
    }
    return h_curr;
}
