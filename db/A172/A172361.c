/*
 * A172361  Number of walks within N^2 (the first quadrant of Z^2) starting
 *          and ending at (0,0), and consisting of n steps taken from {(-1,
 *          -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)}
 *          (king moves).
 * https://oeis.org/A172361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * *
 *            (n+1)*(n+2)^2*(3*n-1)*a(n) * = (n-1)*(n+1)*(9*n*(n+1)+4)*a(n-1)
 *            * + 4*(n-1)*(3*n-2)*(9*n^2+5*n-1)*a(n-2) * +
 *            32*n*(n-1)*(n-2)*(3*n+2)*a(n-3) * * with initial values a(0) =
 *            1, a(1) = 0, a(2) = 3. * The implementation computes the
 *            recurrence iteratively, usi...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)0;
    if (n == 2U) return (seq_t)3;

    seq_t prev3 = (seq_t)1;   /* a(n-3)  */
    seq_t prev2 = (seq_t)0;   /* a(n-2)  */
    seq_t prev1 = (seq_t)3;   /* a(n-1)  */

    for (idx_t i = 3U; i <= n; ++i)
    {
        idx_t ii = i;

        if (sizeof(seq_t) <= 8U) {
            /* use 128‑bit intermediates to avoid overflow */
            __uint128_t coeff1 = (__uint128_t)(ii-1U) * (ii+1U) *
                                 ( (__uint128_t)9U*ii*(ii+1U) + 4U );
            __uint128_t coeff2 = (__uint128_t)4U * (ii-1U) *
                                 (3U*ii-2U) *
                                 ( (__uint128_t)9U*ii*ii +
                                   (__uint128_t)5U*ii - 1U );
            __uint128_t coeff3 = (__uint128_t)32U * ii *
                                 (ii-1U) * (ii-2U) *
                                 (3U*ii+2U);
            __uint128_t denom = (__uint128_t)(ii+1U) *
                                (ii+2U) * (ii+2U) *
                                (3U*ii-1U);

            __uint128_t numer = coeff1 * ( __uint128_t)prev1
                              + coeff2 * ( __uint128_t)prev2
                              + coeff3 * ( __uint128_t)prev3;
            seq_t curr = (seq_t)(numer / denom);

            prev3 = prev2;
            prev2 = prev1;
            prev1 = curr;
        } else {
            /* arbitrary‑precision seq_t arithmetic */
            seq_t coeff1 = (seq_t)(ii-1U) * (seq_t)(ii+1U) *
                           ( ((seq_t)9U * (seq_t)ii * (seq_t)(ii+1U)) + (seq_t)4U );
            seq_t coeff2 = (seq_t)4U * (seq_t)(ii-1U) * (seq_t)(3U*ii-2U) *
                           ( ((seq_t)9U * (seq_t)ii * (seq_t)ii) +
                             (seq_t)5U*ii - (seq_t)1U );
            seq_t coeff3 = (seq_t)32U * (seq_t)ii *
                           (seq_t)(ii-1U) * (seq_t)(ii-2U) *
                           (3U*ii+2U);
            seq_t denom = (seq_t)(ii+1U) *
                          (seq_t)(ii+2U) * (seq_t)(ii+2U) *
                          (3U*ii-1U);

            seq_t numer = coeff1 * prev1 + coeff2 * prev2 + coeff3 * prev3;
            seq_t curr  = numer / denom;

            prev3 = prev2;
            prev2 = prev1;
            prev1 = curr;
        }
    }

    return prev1;
}
