/*
 * A216111  The hyper-Wiener index of the meta-polyphenyl chain with n
 *          hexagons (see the Dou et al. and the Deng references).
 * https://oeis.org/A216111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(-13+14*n+18*n^2+9*n^3)/2
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 132, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sqn = (seq_t)n;

    /* Compute the cubic polynomial: 9*n^3 + 18*n^2 + 14*n - 13 */
    seq_t inner = (seq_t)9 * sqn * sqn * sqn;
    inner += (seq_t)18 * sqn * sqn;
    inner += (seq_t)14 * sqn;
    inner -= (seq_t)13;

    /* Multiply by 3*n and divide by 2 */
    seq_t result = (seq_t)3 * sqn * inner;
    result /= (seq_t)2;

    return result;
}
