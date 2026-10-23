/*
 * A033408  a(n) = floor(88/n).
 * https://oeis.org/A033408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(88 / n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1 the formula holds.  The behaviour for n == 0 is
       irrelevant for the OEIS offset but we guard against a division
       by zero just in case. */
    if (n == 0)
        return (seq_t)0;
    return ((seq_t)88) / ((seq_t)n);
}
