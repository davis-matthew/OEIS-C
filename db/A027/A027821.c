/*
 * A027821  a(n) = 21*(n+1)*binomial(n+6,9).
 * https://oeis.org/A027821
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21 * (n + 1) * binomial(n + 6, 9)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 22, u64 n <= 222, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 3.  For n < 3 we return 0
       (behavior below offset is unspecified). */
    if (n < 3)
        return 0;

    /* Compute binomial(n + 6, 9) multiplicatively. */
    idx_t N = n + 6;
    const idx_t K = 9;
    seq_t bin = 1;
    for (idx_t i = 1; i <= K; ++i) {
        bin *= (seq_t)(N - K + i);
        bin /= (seq_t)i;
    }

    /* Multiply 21*(n+1)*binomial */
    seq_t result = 21;
    result *= (seq_t)(n + 1);
    result *= bin;
    return result;
}
