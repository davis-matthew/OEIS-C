/*
 * A008922  Crystal ball sequence for lattice {E_7}*.
 * https://oeis.org/A008922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (35 +37*n +679*n^2 +392*n^3 +280*n^4 +518*n^5 -14*n^6
 *            +68*n^7)/35
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 514, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: binomial(m,7) with m>=0.  Returns 0 if m<7. */
static seq_t binom7(idx_t m)
{
    if (m < 7)  return 0;
    seq_t res = 1;
    for (int i = 1; i <= 7; ++i)
    {
        /* multiply by (m-7+i) and then divide by i.
           The division is exact because we are computing a binomial. */
        res = res * (m - 7 + (idx_t)i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Coefficients of the numerator polynomial of the rational G.f. */
static const seq_t coeffs[8] = {
    (seq_t)1,  // q0
    (seq_t)49, // q1
    (seq_t)567, // q2
    (seq_t)2263, // q3
    (seq_t)3703, // q4
    (seq_t)2583, // q5
    (seq_t)625, // q6
    (seq_t)1   // q7
};

/* Sequence definition */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (int k = 0; k < 8; ++k)
    {
        seq_t bin = binom7(n + 7 - (idx_t)k);
        sum += coeffs[k] * bin;
    }
    return sum;
}
