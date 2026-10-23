/*
 * A010964  Binomial coefficient C(48,n).
 * https://oeis.org/A010964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(48,n) (the nth binomial coefficient of row 48)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The binomial coefficient is zero for n > 48. */
    if (n > 48) return 0;

    /* Use symmetry C(48,n) = C(48,48-n) to minimize the number of multiplications. */
    idx_t k = n;
    if (k > 48 - k) k = 48 - k;   /* now 0 <= k <= 24 */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply by (48 - k + i) / i in a single step to avoid overflow as much as possible. */
        result = result * ( (seq_t)(48 - k + i) ) / (seq_t)i;
    }
    return result;
}
