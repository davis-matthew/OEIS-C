/*
 * A176260  Period 2: repeat [5, 1].
 * https://oeis.org/A176260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, cons, easy, nonn, mult
 * Formula:   a(n) = 3+2*(-1)^n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period 2: even indices give 5, odd indices give 1 */
    if ((n & 1) == 0)
        return (seq_t)5;
    else
        return (seq_t)1;
}
