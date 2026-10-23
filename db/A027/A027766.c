/*
 * A027766  a(n) = (n+1)*binomial(n+1,6).
 * https://oeis.org/A027766
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*binomial(n+1,6)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is not defined; return 0 in that case. */
    if (n < 5)
        return (seq_t)0;

    /* Compute binomial(n+1,6) multiplicatively to keep values small. */
    seq_t res = 1;
    for (idx_t i = 0; i < 6; ++i) {
        res = res * (seq_t)(n + 1 - i);
        res = res / (seq_t)(i + 1);
    }

    /* Multiply by (n+1) to get the final value. */
    return res * (seq_t)(n + 1);
}
