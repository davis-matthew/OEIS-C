/*
 * A213933  G.f.: (1+x+x^2+2*x^5-2*x^10)/(1-3*x^3).
 * https://oeis.org/A213933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k≥0} 3^k * c(n-3k) where c(m) = 1 for m=0,1,2 = 2 for
 *            m=5 = -2 for m=10 = 0 otherwise This is derived from the
 *            generating function (1 + x + x^2 + 2*x^5 - 2*x^10)/(1-3*x^3)
 * Range:     u8 n <= 16, u16 n <= 31, u32 n <= 61, u64 n <= 121,
 *            u128 n <= 242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;          /* running sum   */
    seq_t pow3   = 1;          /* 3^k           */

    /* iterate over k = 0 .. floor(n/3) */
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        idx_t m = n - 3 * k;

        if (m == 0 || m == 1 || m == 2)           /* c(m) = 1   */
            result += pow3;
        else if (m == 5)                          /* c(m) = 2   */
            result += pow3 + pow3;                /* 2*pow3      */
        else if (m == 10)                         /* c(m) = -2  */
            result -= pow3 + pow3;                /* 2*pow3      */

        /* advance to the next power of 3 */
        pow3 *= 3;
    }
    return result;
}
