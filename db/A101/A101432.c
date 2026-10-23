/*
 * A101432  Each term is the number of letters in the Spanish name of the
 *          previous term.
 * https://oeis.org/A101432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word, changed
 * Formula:   a(0)=1, a(1)=3, a(n)=5-(-1)^n for n>1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    /* For n>1, return 4 if n even, 6 if n odd */
    return (seq_t)((n & 1) ? 6 : 4);
}
