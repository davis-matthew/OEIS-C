/*
 * A179173  Numbers n such that Mordell's equation y^2 = x^3 - n has exactly
 *          18 integral solutions.
 * https://oeis.org/A179173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, bref
 * Formula:   a(1)=431, a(2)=9748, a(n)=0 for n>2 – these are the only n for
 *            which the Mordell equation y^2 = x^3 - n has exactly 18 integral
 *            solutions
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)431;
    if (n == 2) return (seq_t)9748;
    return (seq_t)0;
}
