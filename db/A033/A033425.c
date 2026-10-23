/*
 * A033425  a(n) = floor(10^7/n).
 * https://oeis.org/A033425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(10^7 / n)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* 10^7 fits in any seq_t that can represent the sequence values */
    return ((seq_t)10000000) / ((seq_t)n);
}
