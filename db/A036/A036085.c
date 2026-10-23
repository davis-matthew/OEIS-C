/*
 * A036085  Centered cube numbers: (n+1)^7 + n^7.
 * https://oeis.org/A036085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^7 + n^7 = (2*n+1)*(n^6 + 3*n^5 + 9*n^4 + 13*n^3 +
 *            11*n^2 + 5*n + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 511,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Polynomial coefficients for n^6 + 3*n^5 + 9*n^4 + 13*n^3 + 11*n^2 + 5*n + 1 */
    static const idx_t coef[7] = {1, 5, 11, 13, 9, 3, 1};

    seq_t sum = 0;                /* accumulator for the polynomial */
    seq_t pow = 1;                /* current power of n (starts at n^0) */

    for (idx_t k = 0; k < 7; ++k) {
        sum += (seq_t)coef[k] * pow;   /* add coefficient * n^k */
        if (k < 6)                     /* prepare n^(k+1) for next round */
            pow *= (seq_t)n;
    }

    seq_t factor = ((seq_t)n * 2) + 1;  /* (2*n + 1) */
    return sum * factor;               /* final result */
}
