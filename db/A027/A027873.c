/*
 * A027873  a(n) = Product_{i=1..n} (6^i - 1).
 * https://oeis.org/A027873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (6^i - 1) (with a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start with the empty product a(0) = 1 */
    seq_t result = (seq_t)1;
    if (n == 0)
        return result;

    /* pow6 holds 6^i during the loop */
    seq_t pow6 = (seq_t)6;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Multiply by (6^i - 1) */
        result *= (pow6 - (seq_t)1);

        /* Prepare 6^{i+1} for next iteration */
        pow6 *= (seq_t)6;
    }
    return result;
}
