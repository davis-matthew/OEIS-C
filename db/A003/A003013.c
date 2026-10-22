/*
 * A003013  E.g.f. 1 + x*exp(x) + x^2*exp(2*x).
 * https://oeis.org/A003013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = (n^2 - n)*2^n/4 + n = binomial(n,2)*2^(n-1) + n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t m = (seq_t)n;
    seq_t half_prod;
    if (n % 2 == 0)
        half_prod = (m / 2) * (m - 1);
    else
        half_prod = m * ((m - 1) / 2);

    seq_t pow2 = 1;
    idx_t i;
    for (i = 1; i < n; i++)
        pow2 = pow2 * 2;

    return half_prod * pow2 + m;
}
