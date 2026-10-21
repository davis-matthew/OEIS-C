/*
 * A091994  Numerator of I(n) = sqrt(10)*(Integral_{x=0 to 1/3}
 *          1/(1+x^2)^(n+1/2) dx).
 * https://oeis.org/A091994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 * Recurrence: a(n) = 20*(n-1)*a(n-1) + 9^(n-1) *
 *            ∏_{k=1}^{n-1} (2k-1) * (This is derived from the integral
 *            definition of A091994.)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t a_val = (seq_t)1;   /* current a(n-1), starting with a(1)=1 */
    seq_t f      = (seq_t)1;   /* product of odd numbers up to 2*(n-1)-1;
                               * f = ∏_{k=1}^{n-1} (2k-1)  at each loop start */
    seq_t p9     = (seq_t)1;   /* 9^(n-1), updated each iteration */

    for (idx_t k = 2; k <= n; ++k) {
        /* Prepare 9^(k-1) */
        p9 *= (seq_t)9;

        /* term = 9^(k-1) * ∏_{i=1}^{k-1} (2i-1) */
        seq_t term = p9 * f;

        /* a(k) = 20*(k-1)*a(k-1) + term */
        a_val = a_val * (seq_t)(20 * (k - 1)) + term;

        /* Update product of odds for next iteration:
           f = ∏_{i=1}^{k} (2i-1) = previous f * (2k-1) */
        f *= (seq_t)(2 * k - 1);
    }

    return a_val;
}
