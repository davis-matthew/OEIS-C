/*
 * A082042  a(n) = (n^2 + 1)*n!.
 * https://oeis.org/A082042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + 1) * n! (offset 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes n! iteratively and then multiplies by (n^2 + 1). */

seq_t a(idx_t n)
{
    seq_t fact = 1;

    /* Compute n! */
    for (idx_t i = 1; i <= n; ++i)
        fact = fact * (seq_t)i;

    /* Compute (n^2 + 1) */
    seq_t nsq = ((seq_t)n * (seq_t)n) + 1;

    /* Final result */
    return nsq * fact;
}
