/*
 * A124090  a(n) = C(n,7)-1.
 * https://oeis.org/A124090
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n,7) - 1
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return 0;      /* offset behaviour: undefined below 7 */

    seq_t result = 1;
    /* Compute C(n,7) multiplicatively to keep intermediate values small */
    for (idx_t k = 1; k <= 7; ++k)
    {
        result = result * (seq_t)(n - 7 + k);
        result = result / (seq_t)k;   /* division is exact at each step */
    }

    return result - 1;   /* subtract 1 as per definition */
}
