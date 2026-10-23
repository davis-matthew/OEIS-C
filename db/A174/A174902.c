/*
 * A174902  Denominator of 1 - 1/n^2, using 0 at the pole where n=0.
 * https://oeis.org/A174902
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = n^2 for n≥0, a(−1)=1, a(0)=0 (denominator of 1−1/n^2)
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == -1)                /* special case: denominator of the pole */
        return (seq_t)1;
    if (n == 0)
        return (seq_t)0;
    /* for n ≥ 1 the denominator is simply n^2 */
    seq_t x = (seq_t)n;
    return x * x;
}
