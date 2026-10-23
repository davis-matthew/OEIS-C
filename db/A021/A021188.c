/*
 * A021188  Decimal expansion of 1/184.
 * https://oeis.org/A021188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/184) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    seq_t rem   = 1;    /* remainder before the step */
    seq_t digit = 0;    /* current decimal digit */

    for (i = 0; i <= n; ++i) {
        idx_t prod = (idx_t)rem * 10;          /* 10 * remainder           */
        digit = (seq_t)(prod / 184);           /* integer part of division */
        rem   = (seq_t)(prod % 184);           /* new remainder            */
    }
    return digit;
}
