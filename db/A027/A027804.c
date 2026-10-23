/*
 * A027804  a(n) = 14*(n+1)*binomial(n+4,8).
 * https://oeis.org/A027804
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*(n+1)*binomial(n+4,8) for n >= 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The definition of the sequence is only valid for n >= 4.
     * For smaller indices the behavior is irrelevant; return 0. */
    if (n < 4) return 0;

    /* Compute C(n+4, 8) = Π_{i=1}^{8} (n-4+i) / i
     * The division is exact at each step, so no loss of precision. */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 8; ++i)
    {
        bin = bin * ((seq_t)n - 4 + i) / i;
    }

    /* a(n) = 14 * (n + 1) * bin */
    seq_t result = bin * ((seq_t)(n + 1));
    result = result * ((seq_t)14);
    return result;
}
