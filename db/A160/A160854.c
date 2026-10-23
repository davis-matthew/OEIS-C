/*
 * A160854  Expansion of
 *          (1+147*x+1098*x^2+1638*x^3+632*x^4+59*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = expansion of
 *            (1+147*x+1098*x^2+1638*x^3+632*x^4+59*x^5+x^6)/(1-x)^7
 * Range:     u8 none, u16 n <= 3, u32 n <= 30, u64 n <= 1243, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    static const seq_t coeff[7] = { (seq_t)1, 147, 1098, 1638, 632, 59, 1 };

    seq_t N = n;
    for (idx_t k = 0; k <= 6 && k <= n; ++k)
    {
        seq_t t = N - (seq_t)k + 6;          /* t >= 6 for all remaining k */

        /* Compute binomial(t,6) = t*(t-1)*...*(t-5) / 720 */
        seq_t bin = t;
        bin = bin * (t - 1);
        bin = bin * (t - 2);
        bin = bin * (t - 3);
        bin = bin * (t - 4);
        bin = bin * (t - 5);
        bin = bin / 720;                    /* 6! = 720 */

        sum += coeff[k] * bin;
    }
    return sum;
}
