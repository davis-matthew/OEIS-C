/*
 * A067867  Numbers n such that n and 2^n end with the same 4 digits.
 * https://oeis.org/A067867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 8736 + 10000*(n-1)
 * Range:     u8 none, u16 n <= 6, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1; return 0 for safety */
        return 0;
    seq_t term = 8736;               /* a(1) */
    term += (seq_t)10000 * ((seq_t)n - 1);
    return term;
}
