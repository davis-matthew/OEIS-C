/*
 * A010953  Binomial coefficient C(37,n).
 * https://oeis.org/A010953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(37,n) (C(37,n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n > 37, the binomial coefficient is zero */
    if (n > (idx_t)37) {
        return (seq_t)0;
    }

    seq_t result = (seq_t)1;
    /* Compute multiplicatively to avoid large factorials */
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Multiply before dividing to keep the result integral */
        result = (result * ((seq_t)(37 - (int)i + 1))) / i;
    }
    return result;
}
