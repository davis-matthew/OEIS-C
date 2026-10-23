/*
 * A140093  G.f. satisfies: A(x - x^3) = x^3 - x^9.
 * https://oeis.org/A140093
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is even; a(3)=1; for odd n>3 with * n = 2*k+1,
 *            a(n) = 6 * C(3*k-3, k-2) / k, where C is the binomial
 *            coefficient
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 26, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Even indices are zero. */
    if ((n & 1) == 0)
        return 0;

    /* Special first odd term. */
    if (n == 3)
        return 1;

    /* n = 2*k+1  =>  k = (n-1)/2  */
    idx_t k = (n - 1) >> 1;

    /* Compute the binomial C(3*k-3, k-2) multiplicatively. */
    idx_t N = 3 * k - 3;
    idx_t K = k - 2;   /* k > 1 because n > 3 */
    seq_t binom = 1;

    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i);
        binom = binom * num / (seq_t)i;
    }

    /* Final value: 6 * binom / k */
    seq_t res = binom * (seq_t)6 / (seq_t)k;
    return res;
}
