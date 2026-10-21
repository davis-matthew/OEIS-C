/*
 * A059837  Diagonal T(s,s) of triangle A059836.
 * https://oeis.org/A059837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * binomial(n-1, floor((n-1)/2))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of sequence A059837. */

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence. */
    idx_t m = n - 1;               /* m = n-1 */
    /* factorial(m) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= i;

    /* binomial(m, floor(m/2)) using the multiplicative formula */
    idx_t k = m / 2;               /* floor((n-1)/2) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = m - k + i;
        binom = binom * num;
        binom = binom / i;        /* division is exact at each step */
    }

    return fact * binom;
}
