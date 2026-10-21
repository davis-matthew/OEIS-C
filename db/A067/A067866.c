/*
 * A067866  Numbers n such that n and 2^n end with the same three digits.
 * https://oeis.org/A067866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 736 + 1000*(n-1)
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 the value is
       unspecified; the implementation returns 0 in that case. */
    if (n == 0)
        return (seq_t)0;

    return (seq_t)736 + (seq_t)1000 * ((seq_t)n - 1);
}
