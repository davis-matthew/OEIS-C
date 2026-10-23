/*
 * A161210  Toothpick sequence starting at the outside corner of an infinite
 *          cube from which protrudes a half toothpick.
 * https://oeis.org/A161210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=3, a(n)=7*(n-2) for n>=3
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;
    /* n >= 3 */
    return (seq_t)7 * ((seq_t)n - (seq_t)2);
}
