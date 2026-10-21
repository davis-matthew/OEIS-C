/*
 * A063521  a(n) = n*(7*n^2-4)/3.
 * https://oeis.org/A063521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n^2-4)/3
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 940, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    /* Compute the numerator safely using seq_t arithmetic only. */
    seq_t val = nn * ((seq_t)7 * nn * nn - (seq_t)4) / 3;
    return val;
}
