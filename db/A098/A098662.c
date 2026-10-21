/*
 * A098662  Expansion of e.g.f. BesselI(0,2*sqrt(3)*x) +
 *          BesselI(1,2*sqrt(3)*x)/sqrt(3).
 * https://oeis.org/A098662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * 3^floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t binom = 1;          /* compute C(n, k) multiplicatively */

    for (idx_t i = 1; i <= k; ++i)
    {
        /* binom = binom * (n - k + i) / i  */
        binom = binom * (seq_t)(n - k + i) / (seq_t)i;
    }

    /* multiply by 3^k */
    for (idx_t i = 0; i < k; ++i)
    {
        binom *= 3;
    }

    return binom;
}
