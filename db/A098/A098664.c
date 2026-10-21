/*
 * A098664  Expansion of e.g.f. BesselI(0,4*x)+BesselI(1,4*x)/2.
 * https://oeis.org/A098664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * 4^floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 2;          /* floor(n/2) */
    seq_t binom = 1;

    /* Compute C(n, k) multiplicatively */
    for (idx_t i = 1; i <= k; ++i)
    {
        binom *= (seq_t)(n - k + i);
        binom /= (seq_t)i;
    }

    /* Compute 4^k */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < k; ++i)
    {
        pow4 *= (seq_t)4;
    }

    return binom * pow4;
}
