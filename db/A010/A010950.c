/*
 * A010950  Binomial coefficient C(34,n).
 * https://oeis.org/A010950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(34, n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No terms beyond n = 34 are defined */
    if (n > 34)
        return 0;

    /* Use symmetry to keep the loop as short as possible */
    idx_t k = n;
    if (k > 34 - k)
        k = 34 - k;

    seq_t result = 1;
    /* Multiplicative formula: result *= (34 - i + 1) / i  */
    for (idx_t i = 1; i <= k; ++i)
    {
        unsigned long long tmp =
            (unsigned long long)result * (34 - i + 1);
        tmp /= i;
        result = (seq_t)tmp;
    }
    return result;
}
